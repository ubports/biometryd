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

#ifndef BIOMETRYD_DBUS_STUB_IDENTIFIER_H_
#define BIOMETRYD_DBUS_STUB_IDENTIFIER_H_

#include <biometry/identifier.h>

#include <core/dbus/object.h>
#include <core/dbus/service.h>

namespace biometry
{
namespace dbus
{
namespace stub
{
// Identifier is the dbus stub implementation of biometry::Service.
class Identifier : public biometry::Identifier
{
public:
    // Safe us some typing.
    typedef std::shared_ptr<Identifier> Ptr;

    /// @brief create_for_bus creates a new instance connecting to bus, forwarding incoming calls to impl.
    static Ptr create_for_service_and_object(const core::dbus::Bus::Ptr& bus, const core::dbus::Service::Ptr& service, const core::dbus::Object::Ptr& object);

    // From biometry::Identifier.
    Operation<Identification>::Ptr identify_user(const Application& app, const Reason& reason) override;

private:
    /// @brief Service creates a new instance for the given remote service and object.
    Identifier(const core::dbus::Bus::Ptr& bus, const core::dbus::Service::Ptr& service, const core::dbus::Object::Ptr& object);

    core::dbus::Bus::Ptr bus;
    core::dbus::Service::Ptr service;
    core::dbus::Object::Ptr object;
};
}
}
}

#endif // BIOMETRYD_DBUS_STUB_IDENTIFIER_H_
