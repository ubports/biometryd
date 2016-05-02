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

#include <biometry/dbus/stub/identifier.h>

#include <biometry/dbus/codec.h>
#include <biometry/dbus/interface.h>
#include <biometry/dbus/stub/operation.h>

biometry::dbus::stub::Identifier::Ptr biometry::dbus::stub::Identifier::create_for_service_and_object(
        const core::dbus::Bus::Ptr& bus,
        const core::dbus::Service::Ptr& service,
        const core::dbus::Object::Ptr& object)
{
    return Ptr{new Identifier{bus, service, object}};
}

biometry::Operation<biometry::Identification>::Ptr biometry::dbus::stub::Identifier::identify_user(const Application& app, const Reason& reason)
{
    auto result = object->transact_method<
            biometry::dbus::interface::Identifier::Methods::IdentifyUser,
            biometry::dbus::interface::Identifier::Methods::IdentifyUser::ResultType
    >(app, reason);

    return Operation<Identification>::create_for_object_and_service(bus, service, service->object_for_path(result.value()));
}

biometry::dbus::stub::Identifier::Identifier(const core::dbus::Bus::Ptr& bus, const core::dbus::Service::Ptr& service, const core::dbus::Object::Ptr& object)
    : bus{bus},
      service{service},
      object{object}
{
}
