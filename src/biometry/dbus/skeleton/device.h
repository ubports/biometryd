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

#ifndef BIOMETRYD_DBUS_SKELETON_DEVICE_H_
#define BIOMETRYD_DBUS_SKELETON_DEVICE_H_

#include <biometry/device.h>
#include <biometry/visibility.h>

#include <biometry/dbus/skeleton/identifier.h>
#include <biometry/dbus/skeleton/template_store.h>

#include <biometry/util/once.h>

#include <core/dbus/object.h>
#include <core/dbus/service.h>

namespace biometry
{
namespace dbus
{
namespace skeleton
{

class BIOMETRY_DLL_PUBLIC Device : public biometry::Device
{
public:
    typedef std::shared_ptr<Device> Ptr;

    /// @brief create_for_bus returns a new skeleton::Device instance connected to bus, forwarding calls to impl.
    static Ptr create_for_service_and_object(
            const core::dbus::Bus::Ptr& bus,
            const core::dbus::Service::Ptr& service,
            const core::dbus::Object::Ptr& object,
            const std::shared_ptr<biometry::Device>& impl);

    /// @brief Frees up resources and removes routes to message handlers.
    ~Device();

    // From biometry::Device
    biometry::TemplateStore& template_store() override;
    biometry::Identifier& identifier() override;
    biometry::Verifier& verifier() override;

private:
    /// @brief Device creates a new instance for the given remote service and object;
    Device(const core::dbus::Bus::Ptr& bus, const core::dbus::Service::Ptr& service, const core::dbus::Object::Ptr& object, const std::shared_ptr<biometry::Device>& impl);

    std::shared_ptr<biometry::Device> impl_;

    core::dbus::Bus::Ptr bus_;
    core::dbus::Service::Ptr service_;
    core::dbus::Object::Ptr object_;

    util::Once<std::shared_ptr<biometry::dbus::skeleton::TemplateStore>> template_store_;
    util::Once<std::shared_ptr<biometry::dbus::skeleton::Identifier>> identifier_;
};
}
}
}

#endif // BIOMETRYD_DBUS_SKELETON_DEVICE_H_
