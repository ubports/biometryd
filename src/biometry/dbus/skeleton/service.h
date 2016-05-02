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

#ifndef BIOMETRYD_DBUS_SKELETON_SERVICE_H_
#define BIOMETRYD_DBUS_SKELETON_SERVICE_H_

#include <biometry/service.h>
#include <biometry/visibility.h>

#include <biometry/dbus/skeleton/device.h>

#include <util/once.h>

#include <core/dbus/object.h>
#include <core/dbus/service.h>

namespace biometry
{
namespace dbus
{
namespace skeleton
{
/// @cond
class Device;
/// @endcond

// Service is the dbus SKELETON implementation of biometry::Service.
class BIOMETRY_DLL_PUBLIC Service : public biometry::Service
{
public:
    // Safe us some typing.
    typedef std::shared_ptr<Service> Ptr;

    /// @brief create_for_bus creates a new instance connecting to bus, forwarding incoming calls to impl.
    static Ptr create_for_bus(const core::dbus::Bus::Ptr& bus_, const std::shared_ptr<biometry::Service>& impl_);

    /// @brief Frees up resources and removes routes to message handlers.
    ~Service();

    // From biometry::Service.
    std::shared_ptr<biometry::Device> default_device() const override;

private:
    /// @brief Service creates a new instance for the given remote service and object.
    Service(const core::dbus::Bus::Ptr& bus, const core::dbus::Service::Ptr& service, const core::dbus::Object::Ptr& object, const std::shared_ptr<biometry::Service>& impl);

    std::shared_ptr<biometry::Service> impl_;
    core::dbus::Bus::Ptr bus_;
    core::dbus::Service::Ptr service_;
    core::dbus::Object::Ptr object_;

    util::Once<std::shared_ptr<Device>> default_device_;
};
}
}
}

#endif // BIOMETRYD_DBUS_SKELETON_SERVICE_H_
