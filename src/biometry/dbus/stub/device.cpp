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

#include <biometry/dbus/stub/device.h>

#include <biometry/dbus/interface.h>
#include <biometry/dbus/stub/identifier.h>
#include <biometry/dbus/stub/template_store.h>

/// @brief Device creates a new instance for the given remote service and object;
biometry::dbus::stub::Device::Device(const core::dbus::Bus::Ptr& bus, const core::dbus::Service::Ptr& service, const core::dbus::Object::Ptr& object)
    : bus_{bus},
      service_{service},
      object_{object}
{
}

// From biometry::Device
biometry::TemplateStore& biometry::dbus::stub::Device::template_store()
{
    return *template_store_([this]()
    {
        auto result = object_->invoke_method_synchronously<
                biometry::dbus::interface::Device::Methods::TemplateStore,
                biometry::dbus::interface::Device::Methods::TemplateStore::ResultType
        >();

        return biometry::dbus::stub::TemplateStore::create_for_service_and_object(bus_, service_, service_->object_for_path(result.value()));
    });
}

biometry::Identifier& biometry::dbus::stub::Device::identifier()
{
    return *identifier_([this]()
    {
        auto result = object_->invoke_method_synchronously<
                biometry::dbus::interface::Device::Methods::Identifier,
                biometry::dbus::interface::Device::Methods::Identifier::ResultType
        >();

        return biometry::dbus::stub::Identifier::create_for_service_and_object(bus_, service_, service_->object_for_path(result.value()));
    });
}

biometry::Verifier& biometry::dbus::stub::Device::verifier()
{
    throw std::runtime_error{"Not implemented"};
}
