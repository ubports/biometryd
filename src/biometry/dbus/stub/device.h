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

#ifndef BIOMETRYD_DBUS_STUB_DEVICE_H_
#define BIOMETRYD_DBUS_STUB_DEVICE_H_

#include <biometry/device.h>

#include <util/once.h>

#include <core/dbus/object.h>
#include <core/dbus/service.h>

namespace biometry
{
namespace dbus
{
namespace stub
{
/// @cond
class Identifier;
class TemplateStore;
/// @endcond

class Device : public biometry::Device
{
public:
    /// @brief Device creates a new instance for the given remote service and object;
    Device(const core::dbus::Bus::Ptr& bus, const core::dbus::Service::Ptr& service, const core::dbus::Object::Ptr& object);

    // From biometry::Device
    biometry::TemplateStore& template_store() override;
    biometry::Identifier& identifier() override;
    biometry::Verifier& verifier() override;

private:
    core::dbus::Bus::Ptr bus_;
    core::dbus::Service::Ptr service_;
    core::dbus::Object::Ptr object_;

    util::Once<std::shared_ptr<biometry::dbus::stub::TemplateStore>> template_store_;
    util::Once<std::shared_ptr<biometry::dbus::stub::Identifier>> identifier_;
};
}
}
}

#endif // BIOMETRYD_DBUS_STUB_DEVICE_H_
