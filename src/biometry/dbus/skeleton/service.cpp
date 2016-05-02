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

#include <biometry/dbus/skeleton/service.h>

#include <biometry/dbus/interface.h>

namespace
{
const core::dbus::types::ObjectPath default_device_path("/default_device");
}

biometry::dbus::skeleton::Service::Ptr biometry::dbus::skeleton::Service::create_for_bus(const core::dbus::Bus::Ptr& bus, const std::shared_ptr<biometry::Service>& impl)
{
    auto service = core::dbus::Service::add_service(bus, biometry::dbus::interface::Service::name());
    auto object = service->add_object_for_path(biometry::dbus::interface::Service::path());
    return Ptr{new Service{bus, service, object, impl}};
}

biometry::dbus::skeleton::Service::Service(const core::dbus::Bus::Ptr& bus,
                                           const core::dbus::Service::Ptr& service,
                                           const core::dbus::Object::Ptr& object,
                                           const std::shared_ptr<biometry::Service>& impl)
    : impl_{impl},
      bus_{bus},
      service_{service},
      object_{object}
{
    object_->install_method_handler<biometry::dbus::interface::Service::Methods::DefaultDevice>([this](const core::dbus::Message::Ptr& msg)
    {
        // Ensure that the device gets created on demand.
        default_device();

        auto reply = core::dbus::Message::make_method_return(msg);
        reply->writer() << core::dbus::types::ObjectPath(default_device_path);
        this->bus_->send(reply);
    });
}

biometry::dbus::skeleton::Service::~Service()
{
    object_->uninstall_method_handler<biometry::dbus::interface::Service::Methods::DefaultDevice>();
}

std::shared_ptr<biometry::Device> biometry::dbus::skeleton::Service::default_device() const
{
    return default_device_([this]()
    {
        auto object = service_->add_object_for_path(default_device_path);
        return Device::create_for_service_and_object(bus_, service_, object, impl_->default_device());
    });
}
