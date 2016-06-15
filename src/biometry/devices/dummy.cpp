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

#include <biometry/devices/dummy.h>
#include <biometry/util/configuration.h>
#include <biometry/util/not_implemented.h>

#include <biometry/device_registry.h>

biometry::Operation<biometry::TemplateStore::SizeQuery>::Ptr biometry::devices::Dummy::TemplateStore::size(const biometry::Application&, const biometry::User&)
{
    return std::make_shared<Dummy::Operation<biometry::TemplateStore::SizeQuery>>();
}

biometry::Operation<biometry::TemplateStore::List>::Ptr biometry::devices::Dummy::TemplateStore::list(const biometry::Application&, const biometry::User&)
{
    return std::make_shared<Dummy::Operation<biometry::TemplateStore::List>>();
}

biometry::Operation<biometry::TemplateStore::Enrollment>::Ptr biometry::devices::Dummy::TemplateStore::enroll(const biometry::Application&, const biometry::User&)
{
    return std::make_shared<Dummy::Operation<biometry::TemplateStore::Enrollment>>();
}

biometry::Operation<biometry::TemplateStore::Removal>::Ptr biometry::devices::Dummy::TemplateStore::remove(const biometry::Application&, const biometry::User&, biometry::TemplateStore::TemplateId)
{
    return std::make_shared<Dummy::Operation<biometry::TemplateStore::Removal>>();
}

biometry::Operation<biometry::TemplateStore::Clearance>::Ptr biometry::devices::Dummy::TemplateStore::clear(const biometry::Application&, const biometry::User&)
{
    return std::make_shared<Dummy::Operation<biometry::TemplateStore::Clearance>>();
}

biometry::Operation<biometry::Identification>::Ptr biometry::devices::Dummy::Identifier::identify_user(const biometry::Application&, const biometry::Reason&)
{
    return std::make_shared<Dummy::Operation<biometry::Identification>>();
}

biometry::Operation<biometry::Verification>::Ptr biometry::devices::Dummy::Verifier::verify_user(const Application&, const User&, const Reason&)
{
    return std::make_shared<Dummy::Operation<biometry::Verification>>();
}

biometry::devices::Dummy::Dummy()
{
}

biometry::TemplateStore& biometry::devices::Dummy::template_store()
{
    return template_store_;
}

biometry::Identifier& biometry::devices::Dummy::identifier()
{
    return identifier_;
}

biometry::Verifier& biometry::devices::Dummy::verifier()
{
    return verifier_;
}

namespace
{
struct DummyDescriptor : public biometry::Device::Descriptor
{
    std::shared_ptr<biometry::Device> create(const biometry::util::Configuration&) override
    {
        return std::make_shared<biometry::devices::Dummy>();
    }

    std::string name() const override
    {
        return "Dummy";
    }

    std::string author() const override
    {
        return "Thomas Voß (thomas.voss@canonical.com)";
    }

    std::string description() const override
    {
        return "Dummy is a biometry::Device implementation for tesing purposes.";
    }
};
}

biometry::Device::Descriptor::Ptr biometry::devices::Dummy::make_descriptor()
{
    return std::make_shared<DummyDescriptor>();
}
