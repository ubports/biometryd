/*
 * Copyright (C) 2020 UBports foundation, Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; version 3.
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
 *
 */

#include <arpa/inet.h>

#include <biometry/devices/android.h>
#include <biometry/util/configuration.h>
#include <biometry/util/not_implemented.h>
#include <biometry/util/property_store.h>

#include <biometry/device_registry.h>

#include <iostream>
#include <cstdio>
using namespace std;

std::string IntToStringFingerprintError(int error, int vendorCode){
    switch(error) {
        case ERROR_NO_ERROR: return "ERROR_NO_ERROR";
        case ERROR_HW_UNAVAILABLE: return "ERROR_HW_UNAVAILABLE";
        case ERROR_UNABLE_TO_PROCESS: return "ERROR_UNABLE_TO_PROCESS";
        case ERROR_TIMEOUT: return "ERROR_TIMEOUT";
        case ERROR_NO_SPACE: return "ERROR_NO_SPACE";
        case ERROR_CANCELED: return "ERROR_CANCELED";
        case ERROR_UNABLE_TO_REMOVE: return "ERROR_UNABLE_TO_REMOVE";
        case ERROR_LOCKOUT: return "ERROR_LOCKOUT";
        case ERROR_VENDOR: return "ERROR_VENDOR: " + std::to_string(vendorCode);
        default:
            return "ERROR_NO_ERROR";
    }
}

std::string IntToStringRequestStatus(int error){
    switch(error) {
        case SYS_UNKNOWN: return "SYS_UNKNOWN";
        case SYS_OK: return "SYS_OK";
        case SYS_ENOENT: return "SYS_ENOENT";
        case SYS_EINTR: return "SYS_EINTR";
        case SYS_EIO: return "SYS_EIO";
        case SYS_EAGAIN: return "SYS_EAGAIN";
        case SYS_ENOMEM: return "SYS_ENOMEM";
        case SYS_EACCES: return "SYS_EACCES";
        case SYS_EFAULT: return "SYS_EFAULT";
        case SYS_EBUSY: return "SYS_EBUSY";
        case SYS_EINVAL: return "SYS_EINVAL";
        case SYS_ENOSPC: return "SYS_ENOSPC";
        case SYS_ETIMEDOUT: return "SYS_ETIMEDOUT";
        default:
            return "SYS_OK";
    }
}

namespace
{

class androidEnrollOperation : public biometry::Operation<biometry::TemplateStore::Enrollment>
{
public:
    typename biometry::Operation<biometry::TemplateStore::Enrollment>::Observer::Ptr mobserver;
    int totalrem = 0;

    androidEnrollOperation(UHardwareBiometry hybris_fp_instance, uid_t user_id)
     : hybris_fp_instance{hybris_fp_instance},
       user_id{user_id}
    {
    }

    void start_with_observer(const typename biometry::Operation<biometry::TemplateStore::Enrollment>::Observer::Ptr& observer) override
    {
        mobserver = observer;
        observer->on_started();
        UHardwareBiometryParams fp_params;

        fp_params.enrollresult_cb = enrollresult_cb;
        fp_params.acquired_cb = acquired_cb;
        fp_params.authenticated_cb = authenticated_cb;
        fp_params.error_cb = error_cb;
        fp_params.removed_cb = removed_cb;
        fp_params.enumerate_cb = enumerate_cb;
        fp_params.context = this;

        u_hardware_biometry_setNotify(hybris_fp_instance, &fp_params);

        UHardwareBiometryRequestStatus ret = u_hardware_biometry_enroll(hybris_fp_instance, 0, 60, user_id);
        if (ret != SYS_OK)
            observer->on_failed(IntToStringRequestStatus(ret));
    }

    void cancel() override
    {
        u_hardware_biometry_cancel(hybris_fp_instance);
    }

private:
    UHardwareBiometry hybris_fp_instance;
    uid_t user_id;

    static void acquired_cb(uint64_t, UHardwareBiometryFingerprintAcquiredInfo, int32_t, void *){}
    static void authenticated_cb(uint64_t, uint32_t, uint32_t, void *){}
    static void removed_cb(uint64_t, uint32_t, uint32_t, uint32_t, void *){}
    static void enumerate_cb(uint64_t, uint32_t, uint32_t, uint32_t, void *){}

    static void enrollresult_cb(uint64_t, uint32_t fingerId, uint32_t, uint32_t remaining, void *context)
    {
        if (remaining > 0)
        {
            if (((androidEnrollOperation*)context)->totalrem == 0)
                ((androidEnrollOperation*)context)->totalrem = remaining + 1;
            float raw_value = 1 - ((float)remaining / ((androidEnrollOperation*)context)->totalrem);
            ((androidEnrollOperation*)context)->mobserver->on_progress(biometry::Progress{biometry::Percent::from_raw_value(raw_value), biometry::Dictionary{}});
        } else {
            ((androidEnrollOperation*)context)->mobserver->on_progress(biometry::Progress{biometry::Percent::from_raw_value(1), biometry::Dictionary{}});
            //UHardwareBiometryRequestStatus ret = u_hardware_biometry_postEnroll(((androidEnrollOperation*)context)->hybris_fp_instance);
            //if (ret == SYS_OK)
                ((androidEnrollOperation*)context)->mobserver->on_succeeded(fingerId);
            //else
            //    ((androidEnrollOperation*)context)->mobserver->on_failed(IntToStringRequestStatus(ret));
        }
    }

    static void error_cb(uint64_t, UHardwareBiometryFingerprintError error, int32_t vendorCode, void *context)
    {
        if (error == 0)
            return;

        ((androidEnrollOperation*)context)->mobserver->on_failed(IntToStringFingerprintError(error, vendorCode));
    }

};
class androidRemovalOperation : public biometry::Operation<biometry::TemplateStore::Removal>
{
public:
    typename biometry::Operation<biometry::TemplateStore::Removal>::Observer::Ptr mobserver;

    androidRemovalOperation(UHardwareBiometry hybris_fp_instance, uint32_t finger)
     : hybris_fp_instance{hybris_fp_instance},
       finger{finger}
    {
    }

    void start_with_observer(const typename biometry::Operation<biometry::TemplateStore::Removal>::Observer::Ptr& observer) override
    {
        mobserver = observer;
        observer->on_started();
        UHardwareBiometryParams fp_params;

        fp_params.enrollresult_cb = enrollresult_cb;
        fp_params.acquired_cb = acquired_cb;
        fp_params.authenticated_cb = authenticated_cb;
        fp_params.error_cb = error_cb;
        fp_params.removed_cb = removed_cb;
        fp_params.enumerate_cb = enumerate_cb;
        fp_params.context = this;

        u_hardware_biometry_setNotify(hybris_fp_instance, &fp_params);
        UHardwareBiometryRequestStatus ret = u_hardware_biometry_remove(hybris_fp_instance, 0, finger);
        if (ret != SYS_OK)
            observer->on_failed(IntToStringRequestStatus(ret));
    }

    void cancel() override
    {
        u_hardware_biometry_cancel(hybris_fp_instance);
    }

private:
    UHardwareBiometry hybris_fp_instance;
    uint32_t finger;

    static void enrollresult_cb(uint64_t, uint32_t, uint32_t, uint32_t, void *){}
    static void acquired_cb(uint64_t, UHardwareBiometryFingerprintAcquiredInfo, int32_t, void *){}
    static void authenticated_cb(uint64_t, uint32_t, uint32_t, void *){}
    static void enumerate_cb(uint64_t, uint32_t, uint32_t, uint32_t, void *){}

    static void removed_cb(uint64_t, uint32_t fingerId, uint32_t, uint32_t remaining, void *context)
    {
        if (fingerId == ((androidRemovalOperation*)context)->finger && remaining == 0)
            ((androidRemovalOperation*)context)->mobserver->on_succeeded(fingerId);
    }
    static void error_cb(uint64_t, UHardwareBiometryFingerprintError error, int32_t vendorCode, void *context)
    {
        if (error == 0)
            return;

        ((androidRemovalOperation*)context)->mobserver->on_failed(IntToStringFingerprintError(error, vendorCode));
    }
};

class androidVerificationOperation : public biometry::Operation<biometry::Verification>
{
public:
    typename biometry::Operation<biometry::Verification>::Observer::Ptr mobserver;

    androidVerificationOperation(UHardwareBiometry hybris_fp_instance)
     : hybris_fp_instance{hybris_fp_instance}
    {
    }

    void start_with_observer(const typename biometry::Operation<biometry::Verification>::Observer::Ptr& observer) override
    {
        mobserver = observer;
        observer->on_started();
        UHardwareBiometryParams fp_params;

        fp_params.enrollresult_cb = enrollresult_cb;
        fp_params.acquired_cb = acquired_cb;
        fp_params.authenticated_cb = authenticated_cb;
        fp_params.error_cb = error_cb;
        fp_params.removed_cb = removed_cb;
        fp_params.enumerate_cb = enumerate_cb;
        fp_params.context = this;

        u_hardware_biometry_setNotify(hybris_fp_instance, &fp_params);
        UHardwareBiometryRequestStatus ret = u_hardware_biometry_authenticate(hybris_fp_instance, 0, 0);
        if (ret != SYS_OK)
            observer->on_failed(IntToStringRequestStatus(ret));
    }

    void cancel() override
    {
        u_hardware_biometry_cancel(hybris_fp_instance);
    }

private:
    UHardwareBiometry hybris_fp_instance;

    static void enrollresult_cb(uint64_t, uint32_t, uint32_t, uint32_t, void *){}
    static void acquired_cb(uint64_t, UHardwareBiometryFingerprintAcquiredInfo, int32_t, void *){}
    static void removed_cb(uint64_t, uint32_t, uint32_t, uint32_t, void *){}
    static void enumerate_cb(uint64_t, uint32_t, uint32_t, uint32_t, void *){}

    static void authenticated_cb(uint64_t, uint32_t fingerId, uint32_t, void *context)
    {
        if (fingerId != 0)
            ((androidVerificationOperation*)context)->mobserver->on_succeeded(biometry::Verification::Result::verified);
        else
            ((androidVerificationOperation*)context)->mobserver->on_failed("FINGER_NOT_RECOGNIZED");
    }
    static void error_cb(uint64_t, UHardwareBiometryFingerprintError error, int32_t vendorCode, void *context)
    {
        if (error == 0)
            return;

        ((androidVerificationOperation*)context)->mobserver->on_failed(IntToStringFingerprintError(error, vendorCode));
    }
};

class androidIdentificationOperation : public biometry::Operation<biometry::Identification>
{
public:
    typename biometry::Operation<biometry::Identification>::Observer::Ptr mobserver;

    androidIdentificationOperation(UHardwareBiometry hybris_fp_instance)
    : hybris_fp_instance{hybris_fp_instance}
    {
    }

    void start_with_observer(const typename biometry::Operation<biometry::Identification>::Observer::Ptr& observer) override
    {
        mobserver = observer;
        observer->on_started();
        UHardwareBiometryParams fp_params;

        fp_params.enrollresult_cb = enrollresult_cb;
        fp_params.acquired_cb = acquired_cb;
        fp_params.authenticated_cb = authenticated_cb;
        fp_params.error_cb = error_cb;
        fp_params.removed_cb = removed_cb;
        fp_params.enumerate_cb = enumerate_cb;
        fp_params.context = this;

        u_hardware_biometry_setNotify(hybris_fp_instance, &fp_params);
        UHardwareBiometryRequestStatus ret = u_hardware_biometry_authenticate(hybris_fp_instance, 0, 0);
        if (ret != SYS_OK)
            observer->on_failed(IntToStringRequestStatus(ret));
    }

    void cancel() override
    {
        u_hardware_biometry_cancel(hybris_fp_instance);
    }

private:
    UHardwareBiometry hybris_fp_instance;

    static void enrollresult_cb(uint64_t, uint32_t, uint32_t, uint32_t, void *){}
    static void acquired_cb(uint64_t, UHardwareBiometryFingerprintAcquiredInfo, int32_t, void *){}
    static void removed_cb(uint64_t, uint32_t, uint32_t, uint32_t, void *){}
    static void enumerate_cb(uint64_t, uint32_t, uint32_t, uint32_t, void *){}

    static void authenticated_cb(uint64_t, uint32_t fingerId, uint32_t, void *context)
    {
        if (fingerId != 0)
            ((androidIdentificationOperation*)context)->mobserver->on_succeeded(biometry::User(32011));
        else
            ((androidIdentificationOperation*)context)->mobserver->on_failed("FINGER_NOT_RECOGNIZED");
    }
    static void error_cb(uint64_t, UHardwareBiometryFingerprintError error, int32_t vendorCode, void *context)
    {
        if (error == 0)
        return;

        ((androidIdentificationOperation*)context)->mobserver->on_failed(IntToStringFingerprintError(error, vendorCode));
    }
};

class androidListOperation : public biometry::Operation<biometry::TemplateStore::List>
{
public:
    typename biometry::Operation<biometry::TemplateStore::List>::Observer::Ptr mobserver;
    int totalrem = 0;
    std::vector<uint64_t> result;

    androidListOperation(UHardwareBiometry hybris_fp_instance)
     : hybris_fp_instance{hybris_fp_instance}
    {
    }

    void start_with_observer(const typename biometry::Operation<biometry::TemplateStore::List>::Observer::Ptr& observer) override
    {
        mobserver = observer;
        observer->on_started();
        UHardwareBiometryParams fp_params;

        fp_params.enrollresult_cb = enrollresult_cb;
        fp_params.acquired_cb = acquired_cb;
        fp_params.authenticated_cb = authenticated_cb;
        fp_params.error_cb = error_cb;
        fp_params.removed_cb = removed_cb;
        fp_params.enumerate_cb = enumerate_cb;
        fp_params.context = this;

        u_hardware_biometry_setNotify(hybris_fp_instance, &fp_params);
        UHardwareBiometryRequestStatus ret = u_hardware_biometry_enumerate(hybris_fp_instance);
        if (ret != SYS_OK)
            observer->on_failed(IntToStringRequestStatus(ret));
    }

    void cancel() override
    {
        u_hardware_biometry_cancel(hybris_fp_instance);
    }

private:
    UHardwareBiometry hybris_fp_instance;

    static void enrollresult_cb(uint64_t, uint32_t, uint32_t, uint32_t, void *){}
    static void acquired_cb(uint64_t, UHardwareBiometryFingerprintAcquiredInfo, int32_t, void *){}
    static void authenticated_cb(uint64_t, uint32_t, uint32_t, void *){}
    static void removed_cb(uint64_t, uint32_t, uint32_t, uint32_t, void *){}
    static void enumerate_cb(uint64_t, uint32_t fingerId, uint32_t, uint32_t remaining, void *context)
    {
        auto file = freopen( "/tmp/biometryd.log", "w", stdout );
        cout << "List operation:" << endl;
        if (((androidListOperation*)context)->totalrem == 0)
            ((androidListOperation*)context)->result.clear();
        if (remaining > 0)
        {
            cout << "More than 0 fingerprints remaining..." << endl;
            if (((androidListOperation*)context)->totalrem == 0) {
                ((androidListOperation*)context)->totalrem = remaining + 1;
                cout << "Total number was 0, setting it to" << remaining + 1 << endl;
            }
            float raw_value = 1 - ((float)remaining / ((androidListOperation*)context)->totalrem);
            ((androidListOperation*)context)->mobserver->on_progress(biometry::Progress{biometry::Percent::from_raw_value(raw_value), biometry::Dictionary{}});
            ((androidListOperation*)context)->result.push_back(fingerId);
        } else {
            cout << "No fingerprints remaining..." << endl;
            if (fingerId != 0)
                ((androidListOperation*)context)->result.push_back(fingerId);
            ((androidListOperation*)context)->mobserver->on_progress(biometry::Progress{biometry::Percent::from_raw_value(1), biometry::Dictionary{}});
            ((androidListOperation*)context)->mobserver->on_succeeded(((androidListOperation*)context)->result);
        }
        fclose(file);
    }

    static void error_cb(uint64_t, UHardwareBiometryFingerprintError error, int32_t vendorCode, void *context)
    {
        if (error == 0)
            return;

        ((androidListOperation*)context)->mobserver->on_failed(IntToStringFingerprintError(error, vendorCode));
    }

};

class androidSizeOperation : public biometry::Operation<biometry::TemplateStore::SizeQuery>
{
public:
    typename biometry::Operation<biometry::TemplateStore::SizeQuery>::Observer::Ptr mobserver;
    int totalrem = 0;

    androidSizeOperation(UHardwareBiometry hybris_fp_instance)
     : hybris_fp_instance{hybris_fp_instance}
    {
    }

    void start_with_observer(const typename biometry::Operation<biometry::TemplateStore::SizeQuery>::Observer::Ptr& observer) override
    {
        mobserver = observer;
        observer->on_started();
        UHardwareBiometryParams fp_params;

        fp_params.enrollresult_cb = enrollresult_cb;
        fp_params.acquired_cb = acquired_cb;
        fp_params.authenticated_cb = authenticated_cb;
        fp_params.error_cb = error_cb;
        fp_params.removed_cb = removed_cb;
        fp_params.enumerate_cb = enumerate_cb;
        fp_params.context = this;

        u_hardware_biometry_setNotify(hybris_fp_instance, &fp_params);
        UHardwareBiometryRequestStatus ret = u_hardware_biometry_enumerate(hybris_fp_instance);
        if (ret != SYS_OK)
            observer->on_failed(IntToStringRequestStatus(ret));
    }

    void cancel() override
    {
        u_hardware_biometry_cancel(hybris_fp_instance);
    }

private:
    UHardwareBiometry hybris_fp_instance;

    static void enrollresult_cb(uint64_t, uint32_t, uint32_t, uint32_t, void *){}
    static void acquired_cb(uint64_t, UHardwareBiometryFingerprintAcquiredInfo, int32_t, void *){}
    static void authenticated_cb(uint64_t, uint32_t, uint32_t, void *){}
    static void removed_cb(uint64_t, uint32_t, uint32_t, uint32_t, void *){}
    static void enumerate_cb(uint64_t, uint32_t fingerId, uint32_t, uint32_t remaining, void *context)
    {
        auto file = freopen( "/tmp/biometryd.log", "w", stdout );
        cout << "Size operation:" << endl;
        if (remaining > 0)
        {
            cout << "More than 0 fingerprints remaining..." << endl;
            if (((androidSizeOperation*)context)->totalrem == 0) {
                ((androidSizeOperation*)context)->totalrem = remaining + 1;
                cout << "Total number was 0, setting it to" << remaining + 1 << endl;
            }
            float raw_value = 1 - (remaining / ((androidSizeOperation*)context)->totalrem);
            ((androidSizeOperation*)context)->mobserver->on_progress(biometry::Progress{biometry::Percent::from_raw_value(raw_value), biometry::Dictionary{}});
        } else {
            cout << "No fingerprints remaining..." << endl;
            if (((androidSizeOperation*)context)->totalrem == 0 && fingerId != 0) {
                ((androidSizeOperation*)context)->totalrem++;
                cout << "Total number was 0, but a finger ID was received, setting it to 1" << endl;
            }
            ((androidSizeOperation*)context)->mobserver->on_progress(biometry::Progress{biometry::Percent::from_raw_value(1), biometry::Dictionary{}});
            cout << "Total number of fingerprints:" << ((androidSizeOperation*)context)->totalrem << endl;
            ((androidSizeOperation*)context)->mobserver->on_succeeded(((androidSizeOperation*)context)->totalrem);
        }
        fclose(file);
    }

    static void error_cb(uint64_t, UHardwareBiometryFingerprintError error, int32_t vendorCode, void *context)
    {
        if (error == 0)
            return;

        ((androidSizeOperation*)context)->mobserver->on_failed(IntToStringFingerprintError(error, vendorCode));
    }

};

class androidClearOperation : public biometry::Operation<biometry::TemplateStore::Clearance>
{
public:
    typename biometry::Operation<biometry::TemplateStore::Clearance>::Observer::Ptr mobserver;

    androidClearOperation(UHardwareBiometry hybris_fp_instance)
     : hybris_fp_instance{hybris_fp_instance}
    {
    }

    void start_with_observer(const typename biometry::Operation<biometry::TemplateStore::Clearance>::Observer::Ptr& observer) override
    {
        mobserver = observer;
        observer->on_started();
        UHardwareBiometryParams fp_params;

        fp_params.enrollresult_cb = enrollresult_cb;
        fp_params.acquired_cb = acquired_cb;
        fp_params.authenticated_cb = authenticated_cb;
        fp_params.error_cb = error_cb;
        fp_params.removed_cb = removed_cb;
        fp_params.enumerate_cb = enumerate_cb;
        fp_params.context = this;

        u_hardware_biometry_setNotify(hybris_fp_instance, &fp_params);
        UHardwareBiometryRequestStatus ret = u_hardware_biometry_remove(hybris_fp_instance, 0, 0);
        if (ret != SYS_OK)
            observer->on_failed(IntToStringRequestStatus(ret));
    }

    void cancel() override
    {
        u_hardware_biometry_cancel(hybris_fp_instance);
    }

private:
    UHardwareBiometry hybris_fp_instance;

    static void enrollresult_cb(uint64_t, uint32_t, uint32_t, uint32_t, void *){}
    static void acquired_cb(uint64_t, UHardwareBiometryFingerprintAcquiredInfo, int32_t, void *){}
    static void authenticated_cb(uint64_t, uint32_t, uint32_t, void *){}
    static void enumerate_cb(uint64_t, uint32_t, uint32_t, uint32_t, void *){}

    static void removed_cb(uint64_t, uint32_t, uint32_t, uint32_t remaining, void *context)
    {
        biometry::Void result;
        if (remaining == 0)
            ((androidClearOperation*)context)->mobserver->on_succeeded(result);
    }

    static void error_cb(uint64_t, UHardwareBiometryFingerprintError error, int32_t vendorCode, void *context)
    {
        if (error == 0)
            return;

        ((androidClearOperation*)context)->mobserver->on_failed(IntToStringFingerprintError(error, vendorCode));
    }

};
}

biometry::devices::android::TemplateStore::TemplateStore(UHardwareBiometry hybris_fp_instance)
    : hybris_fp_instance{hybris_fp_instance}
{
}

biometry::Operation<biometry::TemplateStore::SizeQuery>::Ptr biometry::devices::android::TemplateStore::size(const biometry::Application&, const biometry::User&)
{
    return std::make_shared<androidSizeOperation>(hybris_fp_instance);
}

biometry::Operation<biometry::TemplateStore::List>::Ptr biometry::devices::android::TemplateStore::list(const biometry::Application&, const biometry::User&)
{
    return std::make_shared<androidListOperation>(hybris_fp_instance);
}

biometry::Operation<biometry::TemplateStore::Enrollment>::Ptr biometry::devices::android::TemplateStore::enroll(const biometry::Application&, const biometry::User& user)
{
    return std::make_shared<androidEnrollOperation>(hybris_fp_instance, user.id);
}

biometry::Operation<biometry::TemplateStore::Removal>::Ptr biometry::devices::android::TemplateStore::remove(const biometry::Application&, const biometry::User&, biometry::TemplateStore::TemplateId id)
{
    return std::make_shared<androidRemovalOperation>(hybris_fp_instance, id);
}

biometry::Operation<biometry::TemplateStore::Clearance>::Ptr biometry::devices::android::TemplateStore::clear(const biometry::Application&, const biometry::User&)
{
    return std::make_shared<androidClearOperation>(hybris_fp_instance);
}

biometry::devices::android::Identifier::Identifier(UHardwareBiometry hybris_fp_instance)
    : hybris_fp_instance{hybris_fp_instance}
{
}

biometry::Operation<biometry::Identification>::Ptr biometry::devices::android::Identifier::identify_user(const biometry::Application&, const biometry::Reason&)
{
    return std::make_shared<androidIdentificationOperation>(hybris_fp_instance);
}

biometry::devices::android::Verifier::Verifier(UHardwareBiometry hybris_fp_instance)
    : hybris_fp_instance{hybris_fp_instance}
{
}

biometry::Operation<biometry::Verification>::Ptr biometry::devices::android::Verifier::verify_user(const biometry::Application&, const biometry::User&, const biometry::Reason&)
{
    return std::make_shared<androidVerificationOperation>(hybris_fp_instance);
}

biometry::devices::android::android(UHardwareBiometry hybris_fp_instance)
    : template_store_{hybris_fp_instance},
      identifier_{hybris_fp_instance},
      verifier_{hybris_fp_instance}
{
    biometry::util::AndroidPropertyStore store;
    UHardwareBiometryRequestStatus ret = SYS_OK;
    std::string api_level = store.get("ro.product.first_api_level");
    if (api_level.empty())
        api_level = store.get("ro.build.version.sdk");
    if (atoi(api_level.c_str()) <= 27)
        ret = u_hardware_biometry_setActiveGroup(hybris_fp_instance, 0, (char*)"/data/system/users/0/fpdata/");
    else
        ret = u_hardware_biometry_setActiveGroup(hybris_fp_instance, 0, (char*)"/data/vendor_de/0/fpdata/");
    if (ret != SYS_OK)
        printf("setActiveGroup failed: %s\n", IntToStringRequestStatus(ret).c_str());
}

biometry::TemplateStore& biometry::devices::android::template_store()
{
    return template_store_;
}

biometry::Identifier& biometry::devices::android::identifier()
{
    return identifier_;
}

biometry::Verifier& biometry::devices::android::verifier()
{
    return verifier_;
}

namespace
{
struct androidDescriptor : public biometry::Device::Descriptor
{
    std::shared_ptr<biometry::Device> create(const biometry::util::Configuration&) override
    {
        return std::make_shared<biometry::devices::android>(u_hardware_biometry_new());
    }

    std::string name() const override
    {
        return "Android HAL Bridge";
    }

    std::string author() const override
    {
        return "Erfan Abdi (erfangplus@gmail.com)";
    }

    std::string description() const override
    {
        return "android is a biometry::Device implementation for connecting to android fp hal using hybris.";
    }
};
}

biometry::Device::Descriptor::Ptr biometry::devices::android::make_descriptor()
{
    return std::make_shared<androidDescriptor>();
}
