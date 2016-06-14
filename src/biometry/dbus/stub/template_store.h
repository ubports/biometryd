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

#ifndef BIOMETRYD_DBUS_STUB_TEMPLATE_STORE_H_
#define BIOMETRYD_DBUS_STUB_TEMPLATE_STORE_H_

#include <biometry/template_store.h>

#include <core/dbus/object.h>
#include <core/dbus/service.h>

namespace biometry
{
namespace dbus
{
namespace stub
{
// TemplateStore is the dbus stub implementation of biometry::TemplateStore.
class TemplateStore : public biometry::TemplateStore
{
public:
    // Safe us some typing.
    typedef std::shared_ptr<TemplateStore> Ptr;

    /// @brief create_for_bus creates a new instance on the given service and object.
    static Ptr create_for_service_and_object(const core::dbus::Bus::Ptr& bus, const core::dbus::Service::Ptr& service, const core::dbus::Object::Ptr& object);

    // From biometry::Identifier.
    // biometry::Operation<biometry::TemplateStore::Enrollment>
    Operation<SizeQuery>::Ptr size(const Application&, const User&) override;
    Operation<TemplateStore::List>::Ptr list(const Application& app, const User& user) override;
    Operation<Enrollment>::Ptr enroll(const Application&, const User&) override;
    Operation<TemplateStore::Removal>::Ptr remove(const Application& app, const User& user, TemplateStore::TemplateId id) override;
    Operation<Clearance>::Ptr clear(const Application&, const User&) override;

private:
    /// @brief TemplateStore creates a new instance for the given remote service and object.
    TemplateStore(const core::dbus::Bus::Ptr& bus,
                  const core::dbus::Service::Ptr& service,
                  const core::dbus::Object::Ptr& object);

    core::dbus::Bus::Ptr bus;
    core::dbus::Service::Ptr service;
    core::dbus::Object::Ptr object;
};
}
}
}

#endif // BIOMETRYD_DBUS_STUB_TEMPLATE_STORE_H_
