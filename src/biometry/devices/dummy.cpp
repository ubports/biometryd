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

biometry::devices::Dummy::Dummy()
{
}

biometry::TemplateStore& biometry::devices::Dummy::template_store()
{
    util::not_implemented(__PRETTY_FUNCTION__);
}

biometry::Identifier& biometry::devices::Dummy::identifier()
{
    util::not_implemented(__PRETTY_FUNCTION__);
}

biometry::Verifier& biometry::devices::Dummy::verifier()
{
    util::not_implemented(__PRETTY_FUNCTION__);
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
