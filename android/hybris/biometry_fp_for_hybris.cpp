/*
 * Copyright © 2020 UBports foundation Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authored by: Erfan Abdi <erfangplus@gmail.com>
 */
#include <biometry/hardware/biometry.h>

#include <pthread.h>
#include <string>
#include <unistd.h>

// android stuff
#include <hardware/hardware.h>
#include <hardware/fingerprint.h>
#include <hardware/gatekeeper.h>

#include <utils/Log.h>

// Supported fingerprint HAL version
static const uint16_t kVersion = HARDWARE_MODULE_API_VERSION(2, 1);

struct UHardwareBiometry_
{
    UHardwareBiometry_();
    ~UHardwareBiometry_();

    bool init();

    uint64_t setNotify();
    uint64_t preEnroll();
    UHardwareBiometryRequestStatus enroll(uint32_t gid, uint32_t timeoutSec, uint32_t uid);
    UHardwareBiometryRequestStatus postEnroll();
    uint64_t getAuthenticatorId();
    UHardwareBiometryRequestStatus cancel();
    UHardwareBiometryRequestStatus enumerate();
    UHardwareBiometryRequestStatus remove(uint32_t gid, uint32_t fid);
    UHardwareBiometryRequestStatus setActiveGroup(uint32_t gid, char *storePath);
    UHardwareBiometryRequestStatus authenticate(uint64_t operationId, uint32_t gid);

    UHardwareBiometryCallback getCbInstance();
    void notify(const fingerprint_msg_t *msg);
};

struct UHardwareBiometryCallback_
{
    UHardwareBiometryCallback_(UHardwareBiometryParams* params);
    
    UHardwareBiometryEnrollResult enrollresult_cb;
    UHardwareBiometryAcquired acquired_cb;
    UHardwareBiometryAuthenticated authenticated_cb;
    UHardwareBiometryError error_cb;
    UHardwareBiometryRemoved removed_cb;
    UHardwareBiometryEnumerate enumerate_cb;
    
    void* context;
};

fingerprint_device_t* fp_device = nullptr;
UHardwareBiometry hybris_fp_instance = NULL;
UHardwareBiometryCallback hybris_fp_instance_cb = NULL;

UHardwareBiometry_::UHardwareBiometry_()
{

}

UHardwareBiometry_::~UHardwareBiometry_()
{
    if (fp_device != nullptr) {
        fp_device->cancel(fp_device);
    }
}

UHardwareBiometryCallback_::UHardwareBiometryCallback_(UHardwareBiometryParams* params)
    : enrollresult_cb(params->enrollresult_cb),
      acquired_cb(params->acquired_cb),
      authenticated_cb(params->authenticated_cb),
      error_cb(params->error_cb),
      removed_cb(params->removed_cb),
      enumerate_cb(params->enumerate_cb),
      context(params->context)
{

}

UHardwareBiometryFingerprintError VendorErrorFilter(int32_t error, int32_t* vendorCode) {
    *vendorCode = 0;
    switch(error) {
        case FINGERPRINT_ERROR_HW_UNAVAILABLE:
            return ERROR_HW_UNAVAILABLE;
        case FINGERPRINT_ERROR_UNABLE_TO_PROCESS:
            return ERROR_UNABLE_TO_PROCESS;
        case FINGERPRINT_ERROR_TIMEOUT:
            return ERROR_TIMEOUT;
        case FINGERPRINT_ERROR_NO_SPACE:
            return ERROR_NO_SPACE;
        case FINGERPRINT_ERROR_CANCELED:
            return ERROR_CANCELED;
        case FINGERPRINT_ERROR_UNABLE_TO_REMOVE:
            return ERROR_UNABLE_TO_REMOVE;
        default:
            if (error >= FINGERPRINT_ERROR_VENDOR_BASE) {
                // vendor specific code.
                *vendorCode = error - FINGERPRINT_ERROR_VENDOR_BASE;
                return ERROR_VENDOR;
            }
    }
    ALOGE("Unknown error from fingerprint vendor library: %d", error);
    return ERROR_UNABLE_TO_PROCESS;
}

// Translate acquired messages returned by traditional HAL (see fingerprint.h)
UHardwareBiometryFingerprintAcquiredInfo VendorAcquiredFilter(int32_t info, int32_t* vendorCode) {
    *vendorCode = 0;
    switch(info) {
        case FINGERPRINT_ACQUIRED_GOOD:
            return ACQUIRED_GOOD;
        case FINGERPRINT_ACQUIRED_PARTIAL:
            return ACQUIRED_PARTIAL;
        case FINGERPRINT_ACQUIRED_INSUFFICIENT:
            return ACQUIRED_INSUFFICIENT;
        case FINGERPRINT_ACQUIRED_IMAGER_DIRTY:
            return ACQUIRED_IMAGER_DIRTY;
        case FINGERPRINT_ACQUIRED_TOO_SLOW:
            return ACQUIRED_TOO_SLOW;
        case FINGERPRINT_ACQUIRED_TOO_FAST:
            return ACQUIRED_TOO_FAST;
        default:
            if (info >= FINGERPRINT_ACQUIRED_VENDOR_BASE) {
                // vendor specific code.
                *vendorCode = info - FINGERPRINT_ACQUIRED_VENDOR_BASE;
                return ACQUIRED_VENDOR;
            }
    }
    ALOGE("Unknown acquiredmsg from fingerprint vendor library: %d", info);
    return ACQUIRED_INSUFFICIENT;
}

UHardwareBiometryRequestStatus ErrorFilter(int32_t error) {
    switch(error) {
        case 0: return SYS_OK;
        case -2: return SYS_ENOENT;
        case -4: return SYS_EINTR;
        case -5: return SYS_EIO;
        case -11: return SYS_EAGAIN;
        case -12: return SYS_ENOMEM;
        case -13: return SYS_EACCES;
        case -14: return SYS_EFAULT;
        case -16: return SYS_EBUSY;
        case -22: return SYS_EINVAL;
        case -28: return SYS_ENOSPC;
        case -110: return SYS_ETIMEDOUT;
        default:
            ALOGE("An unknown error returned from fingerprint vendor library: %d", error);
            return SYS_UNKNOWN;
    }
}

int gatekeeper_device_initialize(gatekeeper_device_t **dev) {
    int ret;
    const hw_module_t *mod;
    ret = hw_get_module_by_class(GATEKEEPER_HARDWARE_MODULE_ID, NULL, &mod);
    
    if (ret!=0) {
        printf("failed to get hw module\n");
        return ret;
    }
    
    ret = gatekeeper_open(mod, dev);
    
    if (ret!=0)
        printf("failed to open gatekeeper\n");
    return ret;
}

bool UHardwareBiometry_::init()
{
    int err;
    const hw_module_t *hw_mdl = nullptr;
    ALOGD("Opening fingerprint hal library...");
    if (0 != (err = hw_get_module(FINGERPRINT_HARDWARE_MODULE_ID, &hw_mdl))) {
        ALOGE("Can't open fingerprint HW Module, error: %d", err);
        return false;
    }
    
    if (hw_mdl == nullptr) {
        ALOGE("No valid fingerprint module");
        return false;
    }
    
    fingerprint_module_t const *module =
    reinterpret_cast<const fingerprint_module_t*>(hw_mdl);
    if (module->common.methods->open == nullptr) {
        ALOGE("No valid open method");
        return false;
    }
    
    hw_device_t *device = nullptr;
    
    if (0 != (err = module->common.methods->open(hw_mdl, nullptr, &device))) {
        ALOGE("Can't open fingerprint methods, error: %d", err);
        return false;
    }
    
    if (kVersion != device->version) {
        // enforce version on new devices because of HIDL@2.1 translation layer
        ALOGE("Wrong fp version. Expected %d, got %d", kVersion, device->version);
        return false;
    }

    fp_device = reinterpret_cast<fingerprint_device_t*>(device);

    return true;
}

uint64_t UHardwareBiometry_::setNotify()
{
    if (fp_device == nullptr) {
        ALOGE("Unable to get FP device\n");
        return 0;
    }
    return fp_device->set_notify(fp_device, notify);
}

uint64_t UHardwareBiometry_::preEnroll()
{
    if (fp_device == nullptr) {
        ALOGE("Unable to get FP device\n");
        return 0;
    }

    return fp_device->pre_enroll(fp_device);
}

UHardwareBiometryRequestStatus UHardwareBiometry_::enroll(uint32_t gid, uint32_t timeoutSec, uint32_t user_id)
{
    if (fp_device == nullptr) {
        ALOGE("Unable to get FP device\n");
        return SYS_UNKNOWN;
    }
    uint8_t *auth_token;
    uint32_t auth_token_len;
    int ret = 0;
    uint64_t challange = fp_device->pre_enroll(fp_device);
    std::string Password = "default_password";
    bool should_reenroll = false;
    gatekeeper_device_t *gk_device;
    ret = gatekeeper_device_initialize(&gk_device);
    if (ret) {
        ALOGE("Unable to get Gatekeeper hal\n");
        return SYS_UNKNOWN;
    }
    uint8_t* PwdHandle;
    uint32_t PwdHandle_len;

    ret = gk_device->enroll(gk_device, user_id, NULL, 0, NULL, 0,
                            (const uint8_t *)Password.c_str(), Password.size(),
                            &PwdHandle, &PwdHandle_len);
    
    if (ret) {
        ALOGE("Unable to Enroll on Gatekeeper\n");
        return SYS_UNKNOWN;
    }
    
    ret = gk_device->verify(gk_device, user_id, challange, (const uint8_t *)PwdHandle, PwdHandle_len,
                            (const uint8_t *)Password.c_str(), (uint32_t)Password.size(), &auth_token,
                            &auth_token_len, &should_reenroll);

    if (ret) {
        ALOGE("Unable to Verify on Gatekeeper\n");
        return SYS_UNKNOWN;
    }

    return ErrorFilter(fp_device->enroll(fp_device, (const hw_auth_token_t *)auth_token, gid, timeoutSec));
}

UHardwareBiometryRequestStatus UHardwareBiometry_::postEnroll()
{
    if (fp_device == nullptr) {
        ALOGE("Unable to get FP device\n");
        return SYS_UNKNOWN;
    }
    
    return ErrorFilter(fp_device->post_enroll(fp_device));
}

uint64_t UHardwareBiometry_::getAuthenticatorId()
{
    if (fp_device == nullptr) {
        ALOGE("Unable to get FP device\n");
        return 0;
    }
    
    return fp_device->get_authenticator_id(fp_device);
}

UHardwareBiometryRequestStatus UHardwareBiometry_::cancel()
{
    if (fp_device == nullptr) {
        ALOGE("Unable to get FP device\n");
        return SYS_UNKNOWN;
    }
    
    return ErrorFilter(fp_device->cancel(fp_device));
}

UHardwareBiometryRequestStatus UHardwareBiometry_::enumerate()
{
    if (fp_device == nullptr) {
        ALOGE("Unable to get FP device\n");
        return SYS_UNKNOWN;
    }
    
    return ErrorFilter(fp_device->enumerate(fp_device));
}

UHardwareBiometryRequestStatus UHardwareBiometry_::remove(uint32_t gid, uint32_t fid)
{
    if (fp_device == nullptr) {
        ALOGE("Unable to get FP device\n");
        return SYS_UNKNOWN;
    }
    
    return ErrorFilter(fp_device->remove(fp_device, gid, fid));
}

UHardwareBiometryRequestStatus UHardwareBiometry_::setActiveGroup(uint32_t gid, char *storePath)
{
    if (fp_device == nullptr) {
        ALOGE("Unable to get FP device\n");
        return SYS_UNKNOWN;
    }
    
    return ErrorFilter(fp_device->set_active_group(fp_device, gid, storePath));
}

UHardwareBiometryRequestStatus UHardwareBiometry_::authenticate(uint64_t operationId, uint32_t gid)
{
    if (fp_device == nullptr) {
        ALOGE("Unable to get FP device\n");
        return SYS_UNKNOWN;
    }
    
    return ErrorFilter(fp_device->authenticate(fp_device, operationId, gid));
}

UHardwareBiometryCallback UHardwareBiometry_::getCbInstance() {
    return hybris_fp_instance_cb;
}

void UHardwareBiometry_::notify(const fingerprint_msg_t *msg)
{
    UHardwareBiometryCallback hybris_fp_instance_cb = static_cast<UHardwareBiometryCallback>(UHardwareBiometry_::getCbInstance());
    const uint64_t devId = 0; // Multiple fp sensors not supported so always set 0
    switch (msg->type) {
        case FINGERPRINT_ERROR: {
            int32_t vendorCode = 0;
            UHardwareBiometryFingerprintError result =
                VendorErrorFilter(msg->data.error, &vendorCode);
            ALOGD("onError(%d)", result);
            if (hybris_fp_instance_cb && hybris_fp_instance_cb->error_cb) {
                hybris_fp_instance_cb->error_cb(devId,
                                                result,
                                                vendorCode,
                                                hybris_fp_instance_cb->context);
            }
        }
            break;
        case FINGERPRINT_ACQUIRED: {
            int32_t vendorCode = 0;
            UHardwareBiometryFingerprintAcquiredInfo result =
                VendorAcquiredFilter(msg->data.acquired.acquired_info, &vendorCode);
            ALOGD("onAcquired(%d)", result);
            if (hybris_fp_instance_cb && hybris_fp_instance_cb->acquired_cb) {
                hybris_fp_instance_cb->acquired_cb(devId,
                                                   result,
                                                   vendorCode,
                                                   hybris_fp_instance_cb->context);
            }
        }
            break;
        case FINGERPRINT_TEMPLATE_ENROLLING:
            ALOGD("onEnrollResult(fid=%d, gid=%d, rem=%d)",
                  msg->data.enroll.finger.fid,
                  msg->data.enroll.finger.gid,
                  msg->data.enroll.samples_remaining);
            if (hybris_fp_instance_cb && hybris_fp_instance_cb->enrollresult_cb) {
                hybris_fp_instance_cb->enrollresult_cb(devId,
                                                       msg->data.enroll.finger.fid,
                                                       msg->data.enroll.finger.gid,
                                                       msg->data.enroll.samples_remaining,
                                                       hybris_fp_instance_cb->context);
            }
            break;
        case FINGERPRINT_TEMPLATE_REMOVED:
            ALOGD("onRemove(fid=%d, gid=%d)",
                  msg->data.removed.finger.fid,
                  msg->data.removed.finger.gid);
            if (hybris_fp_instance_cb && hybris_fp_instance_cb->removed_cb) {
                hybris_fp_instance_cb->removed_cb(devId,
                                                  msg->data.removed.finger.fid,
                                                  msg->data.removed.finger.gid,
                                                  0,
                                                  hybris_fp_instance_cb->context);
            }
            break;
        case FINGERPRINT_AUTHENTICATED:
            ALOGD("onAuthenticated(fid=%d, gid=%d)",
                  msg->data.authenticated.finger.fid,
                  msg->data.authenticated.finger.gid);
            if (hybris_fp_instance_cb && hybris_fp_instance_cb->authenticated_cb) {
                hybris_fp_instance_cb->authenticated_cb(devId,
                                                        msg->data.authenticated.finger.fid,
                                                        msg->data.authenticated.finger.gid,
                                                        hybris_fp_instance_cb->context);
            }
            break;
        case FINGERPRINT_TEMPLATE_ENUMERATING:
            ALOGD("onEnumerate(fid=%d, gid=%d, rem=%d)",
                  msg->data.enumerated.finger.fid,
                  msg->data.enumerated.finger.gid,
                  msg->data.enumerated.remaining_templates);
            if (hybris_fp_instance_cb && hybris_fp_instance_cb->enumerate_cb) {
                hybris_fp_instance_cb->enumerate_cb(devId,
                                                    msg->data.enumerated.finger.fid,
                                                    msg->data.enumerated.finger.gid,
                                                    msg->data.enumerated.remaining_templates,
                                                    hybris_fp_instance_cb->context);
            }
            break;
    }
}

/////////////////////////////////////////////////////////////////////
// Implementation of the C API

UHardwareBiometry u_hardware_biometry_new()
{
    if (hybris_fp_instance != NULL)
        return NULL;

    UHardwareBiometry u_hardware_biometry = new UHardwareBiometry_();
    hybris_fp_instance = u_hardware_biometry;

    // Try ten times to initialize the FP HAL interface,
    // sleeping for 200ms per iteration in case of issues.
    for (unsigned int i = 0; i < 50; i++)
        if (u_hardware_biometry->init())
            return hybris_fp_instance = u_hardware_biometry;
        else
            // Sleep for some time and leave some time for the system
            // to finish initialization.
            ::usleep(200 * 1000);

    // This is the error case, as we did not succeed in initializing the FP interface.
    delete u_hardware_biometry;
    return hybris_fp_instance;
}

uint64_t u_hardware_biometry_setNotify(UHardwareBiometry self, UHardwareBiometryParams *params)
{
    UHardwareBiometryCallback u_hardware_biometry_cb = new UHardwareBiometryCallback_(params);
    hybris_fp_instance_cb = u_hardware_biometry_cb;

    return self->setNotify();
}

uint64_t u_hardware_biometry_preEnroll(UHardwareBiometry self)
{
    return self->preEnroll();
}

UHardwareBiometryRequestStatus u_hardware_biometry_enroll(UHardwareBiometry self, uint32_t gid, uint32_t timeoutSec, uint32_t uid)
{
    return self->enroll(gid, timeoutSec, uid);
}

UHardwareBiometryRequestStatus u_hardware_biometry_postEnroll(UHardwareBiometry self)
{
    return self->postEnroll();
}

uint64_t u_hardware_biometry_getAuthenticatorId(UHardwareBiometry self)
{
    return self->getAuthenticatorId();
}

UHardwareBiometryRequestStatus u_hardware_biometry_cancel(UHardwareBiometry self)
{
    return self->cancel();
}

UHardwareBiometryRequestStatus u_hardware_biometry_enumerate(UHardwareBiometry self)
{
    return self->enumerate();
}

UHardwareBiometryRequestStatus u_hardware_biometry_remove(UHardwareBiometry self, uint32_t gid, uint32_t fid)
{
    return self->remove(gid, fid);
}

UHardwareBiometryRequestStatus u_hardware_biometry_setActiveGroup(UHardwareBiometry self, uint32_t gid, char *storePath)
{
    return self->setActiveGroup(gid, storePath);
}

UHardwareBiometryRequestStatus u_hardware_biometry_authenticate(UHardwareBiometry self, uint64_t operationId, uint32_t gid)
{
    return self->authenticate(operationId, gid);
}
