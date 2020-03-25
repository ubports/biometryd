/*
 * Copyright (C) 2016 Canonical, Ltd.
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
 * Authored by: Thomas Voß <thomas.voss@canonical.com>
 *
 */

#include <biometry/devices/android.h>
#include <biometry/util/configuration.h>
#include <biometry/util/not_implemented.h>

#include <biometry/device_registry.h>

namespace
{
template<typename T>
class androidOperation : public biometry::Operation<T>
{
public:
    void start_with_observer(const typename biometry::Operation<T>::Observer::Ptr& observer) override
    {
        observer->on_started();
        typename biometry::Operation<T>::Result result{};
        observer->on_succeeded(result);
    }

    void cancel() override
    {

    }
};
}

biometry::Operation<biometry::TemplateStore::SizeQuery>::Ptr biometry::devices::android::TemplateStore::size(const biometry::Application&, const biometry::User&)
{
    return std::make_shared<androidOperation<biometry::TemplateStore::SizeQuery>>();
}

biometry::Operation<biometry::TemplateStore::List>::Ptr biometry::devices::android::TemplateStore::list(const biometry::Application&, const biometry::User&)
{
    return std::make_shared<androidOperation<biometry::TemplateStore::List>>();
}

biometry::Operation<biometry::TemplateStore::Enrollment>::Ptr biometry::devices::android::TemplateStore::enroll(const biometry::Application&, const biometry::User&)
{
    return std::make_shared<androidOperation<biometry::TemplateStore::Enrollment>>();
}

biometry::Operation<biometry::TemplateStore::Removal>::Ptr biometry::devices::android::TemplateStore::remove(const biometry::Application&, const biometry::User&, biometry::TemplateStore::TemplateId)
{
    return std::make_shared<androidOperation<biometry::TemplateStore::Removal>>();
}

biometry::Operation<biometry::TemplateStore::Clearance>::Ptr biometry::devices::android::TemplateStore::clear(const biometry::Application&, const biometry::User&)
{
    return std::make_shared<androidOperation<biometry::TemplateStore::Clearance>>();
}

biometry::Operation<biometry::Identification>::Ptr biometry::devices::android::Identifier::identify_user(const biometry::Application&, const biometry::Reason&)
{
    return std::make_shared<androidOperation<biometry::Identification>>();
}

biometry::Operation<biometry::Verification>::Ptr biometry::devices::android::Verifier::verify_user(const Application&, const User&, const Reason&)
{
    return std::make_shared<androidOperation<biometry::Verification>>();
}

biometry::devices::android::android()
{
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
        return std::make_shared<biometry::devices::android>();
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
