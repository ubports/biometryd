/*
 * Copyright © 2020 Canonical Ltd.
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
#include <string.h>
#include <unistd.h>

// android stuff
#include <android/hardware/biometrics/fingerprint/2.1/IBiometricsFingerprint.h>

#include <utils/Log.h>

using android::OK;
using android::sp;
using android::wp;
using android::status_t;

using android::hardware::biometrics::fingerprint::V2_1::IBiometricsFingerprint;
using android::hardware::biometrics::fingerprint::V2_1::IBiometricsFingerprintClientCallback;
using android::hardware::biometrics::fingerprint::V2_1::RequestStatus;
using android::hardware::biometrics::fingerprint::V2_1::FingerprintAcquiredInfo;
using android::hardware::biometrics::fingerprint::V2_1::FingerprintError;
using android::hardware::Return;
using android::hardware::Void;
using android::hardware::hidl_vec;
using android::hardware::hidl_string;

using android::hidl::base::V1_0::IBase;

sp<IBiometricsFingerprint> fpHal = nullptr;

struct UHardwareBiometry_
{
    UHardwareBiometry_();
    ~UHardwareBiometry_();

    bool init();

    uint64_t setNotify();
    uint64_t preEnroll();
    UHardwareBiometryRequestStatus enroll(uint8_t *hat, uint32_t gid, uint32_t timeoutSec);
    UHardwareBiometryRequestStatus postEnroll();
    uint64_t getAuthenticatorId();
    UHardwareBiometryRequestStatus cancel();
    UHardwareBiometryRequestStatus enumerate();
    UHardwareBiometryRequestStatus remove(uint32_t gid, uint32_t fid);
    UHardwareBiometryRequestStatus setActiveGroup(uint32_t gid, char *storePath);
    UHardwareBiometryRequestStatus authenticate(uint64_t operationId, uint32_t gid);
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


namespace
{
UHardwareBiometry hybris_fp_instance = NULL;
UHardwareBiometryCallback hybris_fp_instance_cb = NULL;

struct BiometricsFingerprintClientCallback : public IBiometricsFingerprintClientCallback {
    Return<void> onEnrollResult(uint64_t deviceId, uint32_t fingerId,
                                uint32_t groupId, uint32_t remaining) override;
    Return<void> onAcquired(uint64_t deviceId, FingerprintAcquiredInfo acquiredInfo,
                            int32_t vendorCode) override;
    Return<void> onAuthenticated(uint64_t deviceId, uint32_t fingerId, uint32_t groupId,
                                 const hidl_vec<uint8_t>& token) override;
    Return<void> onError(uint64_t deviceId, FingerprintError error,
                         int32_t vendorCode) override;
    Return<void> onRemoved(uint64_t deviceId, uint32_t fingerId,
                           uint32_t groupId, uint32_t remaining) override;
    Return<void> onEnumerate(uint64_t deviceId, uint32_t fingerId,
                             uint32_t groupId, uint32_t remaining) override;
};

Return<void> BiometricsFingerprintClientCallback::onEnrollResult(uint64_t deviceId,
    uint32_t fingerId, uint32_t groupId, uint32_t remaining)
{
    if (hybris_fp_instance_cb && hybris_fp_instance_cb->enrollresult_cb) {
        hybris_fp_instance_cb->enrollresult_cb(deviceId, fingerId, groupId, remaining, hybris_fp_instance_cb->context);
    }
    return Void();
}


//TODO: Add more

}

UHardwareBiometry_::UHardwareBiometry_()
{

}

UHardwareBiometry_::~UHardwareBiometry_()
{
    if (fpHal != nullptr) {
        fpHal->cancel();
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

bool UHardwareBiometry_::init()
{
    /* Initializes the FP service handle. */
    fpHal = IBiometricsFingerprint::getService();

    if (fpHal == nullptr) {
        ALOGE("Unable to get FP service\n");
        return false;
    }

    return true;
}

uint64_t UHardwareBiometry_::setNotify()
{
    if (fpHal == nullptr) {
        ALOGE("Unable to get FP service\n");
        return 0;
    }

    sp<IBiometricsFingerprintClientCallback> fpCbIface = new BiometricsFingerprintClientCallback();
    return fpHal->setNotify(fpCbIface);
}

uint64_t UHardwareBiometry_::preEnroll()
{
    if (fpHal == nullptr) {
        ALOGE("Unable to get FP service\n");
        return 0;
    }

    return fpHal->preEnroll();
}

//TODO: Add more

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
    if (hybris_fp_instance_cb != NULL)
        return 0;

    UHardwareBiometryCallback u_hardware_biometry_cb = new UHardwareBiometryCallback_(params);
    hybris_fp_instance_cb = u_hardware_biometry_cb;

    return self->setNotify();
}

uint64_t u_hardware_biometry_preEnroll(UHardwareBiometry self)
{
    return self->preEnroll();
}

//TODO: Add more
