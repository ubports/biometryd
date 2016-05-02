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

#include <biometry/dbus/stub/service.h>

#include <biometry/dbus/interface.h>
#include <biometry/dbus/stub/device.h>

biometry::dbus::stub::Service::Ptr biometry::dbus::stub::Service::create_for_bus(const core::dbus::Bus::Ptr& bus)
{
    auto service = core::dbus::Service::use_service(bus, biometry::dbus::interface::Service::name());
    auto object = service->object_for_path(biometry::dbus::interface::Service::path());
    return Ptr{new Service{bus, service, object}};
}

// From biometry::Service.
std::shared_ptr<biometry::Device> biometry::dbus::stub::Service::default_device() const
{
    auto result = object->transact_method<
            biometry::dbus::interface::Service::Methods::DefaultDevice,
            biometry::dbus::interface::Service::Methods::DefaultDevice::ResultType
    >();

    if (result.is_error())
        throw std::runtime_error{result.error().print()};

    return std::make_shared<biometry::dbus::stub::Device>(bus, service, service->object_for_path(result.value()));
}

biometry::dbus::stub::Service::Service(const core::dbus::Bus::Ptr& bus, const core::dbus::Service::Ptr& service, const core::dbus::Object::Ptr& object)
    : bus{bus},
      service{service},
      object{object}
{
}
