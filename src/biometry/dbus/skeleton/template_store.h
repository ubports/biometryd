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

#ifndef BIOMETRYD_DBUS_SKELETON_TEMPLATE_STORE_H_
#define BIOMETRYD_DBUS_SKELETON_TEMPLATE_STORE_H_

#include <biometry/template_store.h>

#include <util/synchronized.h>

#include <core/dbus/object.h>
#include <core/dbus/service.h>

#include <unordered_map>

namespace biometry
{
namespace dbus
{
namespace skeleton
{
/// @cond
class Device;
/// @endcond

// TemplateStore is the dbus SKELETON implementation of biometry::TemplateStore.
class TemplateStore : public biometry::TemplateStore
{
public:
    // Safe us some typing
    typedef std::shared_ptr<TemplateStore> Ptr;

    /// @brief create_for_bus returns a new skeleton::TemplateStore instance connected to bus, forwarding calls to impl.
    static Ptr create_for_service_and_object(const core::dbus::Bus::Ptr& bus, const core::dbus::Service::Ptr& service, const core::dbus::Object::Ptr& object, const std::reference_wrapper<biometry::TemplateStore>& impl);

    /// @brief Frees up resources and uninstall message handlers.
    ~TemplateStore();

    // From biometry::Identifier.
    // biometry::Operation<biometry::TemplateStore::Enrollment>
    Operation<SizeQuery>::Ptr size(const Application&, const User&) override;
    Operation<Enrollment>::Ptr enroll(const Application&, const User&) override;
    Operation<Clearance>::Ptr clear(const Application&, const User&) override;

private:
    typedef util::Synchronized<std::unordered_map<core::dbus::types::ObjectPath, Operation<SizeQuery>::Ptr>> SizeOps;
    typedef util::Synchronized<std::unordered_map<core::dbus::types::ObjectPath, Operation<Enrollment>::Ptr>> EnrollOps;
    typedef util::Synchronized<std::unordered_map<core::dbus::types::ObjectPath, Operation<Clearance>::Ptr>> ClearOps;

    /// @brief TemplateStore creates a new instance for the given remote service and object.
    TemplateStore(const core::dbus::Bus::Ptr& bus, const core::dbus::Service::Ptr& service, const core::dbus::Object::Ptr& object, const std::reference_wrapper<biometry::TemplateStore>& impl);


    std::reference_wrapper<biometry::TemplateStore> impl;

    core::dbus::Bus::Ptr bus;
    core::dbus::Service::Ptr service;
    core::dbus::Object::Ptr object;

    struct
    {
        SizeOps size;
        EnrollOps enroll;
        ClearOps clear;
    } ops;
};
}
}
}

#endif // BIOMETRYD_DBUS_SKELETON_TEMPLATE_STORE_H_
