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

#include <biometry/dbus/stub/template_store.h>

#include <biometry/application.h>
#include <biometry/user.h>

#include <biometry/dbus/codec.h>
#include <biometry/dbus/interface.h>
#include <biometry/dbus/stub/operation.h>

biometry::dbus::stub::TemplateStore::Ptr biometry::dbus::stub::TemplateStore::create_for_service_and_object(
        const core::dbus::Bus::Ptr& bus,
        const core::dbus::Service::Ptr& service,
        const core::dbus::Object::Ptr& object)
{
    return Ptr{new TemplateStore{bus, service, object}};
}

biometry::Operation<biometry::TemplateStore::SizeQuery>::Ptr biometry::dbus::stub::TemplateStore::size(const biometry::Application& app, const biometry::User& user)
{
    auto result = object->transact_method<
            biometry::dbus::interface::TemplateStore::Methods::Size,
            biometry::dbus::interface::TemplateStore::Methods::Size::ResultType
    >(app, user);

    return Operation<SizeQuery>::create_for_object_and_service(bus, service, service->object_for_path(result.value()));
}

biometry::Operation<biometry::TemplateStore::Enrollment>::Ptr biometry::dbus::stub::TemplateStore::enroll(const biometry::Application& app, const biometry::User& user)
{
    auto result = object->transact_method<
            biometry::dbus::interface::TemplateStore::Methods::Enroll,
            biometry::dbus::interface::TemplateStore::Methods::Enroll::ResultType
    >(app, user);

    return Operation<Enrollment>::create_for_object_and_service(bus, service, service->object_for_path(result.value()));
}

biometry::Operation<biometry::TemplateStore::Clearance>::Ptr biometry::dbus::stub::TemplateStore::clear(const biometry::Application& app, const biometry::User& user)
{
    auto result = object->transact_method<
            biometry::dbus::interface::TemplateStore::Methods::Clear,
            biometry::dbus::interface::TemplateStore::Methods::Clear::ResultType
    >(app, user);

    return Operation<Clearance>::create_for_object_and_service(bus, service, service->object_for_path(result.value()));
}

biometry::dbus::stub::TemplateStore::TemplateStore(const core::dbus::Bus::Ptr& bus, const core::dbus::Service::Ptr& service, const core::dbus::Object::Ptr& object)
    : bus{bus},
      service{service},
      object{object}
{
}
