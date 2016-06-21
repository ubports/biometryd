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

#ifndef BIOMETRYD_DBUS_SKELETON_DAEMON_CREDENTIALS_RESOLVER_H_
#define BIOMETRYD_DBUS_SKELETON_DAEMON_CREDENTIALS_RESOLVER_H_

#include <biometry/dbus/skeleton/credentials_resolver.h>

#include <core/dbus/bus.h>
#include <core/dbus/message.h>

namespace biometry
{
namespace dbus
{
namespace skeleton
{
/// @brief CredentialsResolver resolves incoming messages to RequestVerifier::Credentials.
class DaemonCredentialsResolver : public CredentialsResolver
{
public:
    /// @brief Initializes a new instance with the given bus connection.
    explicit DaemonCredentialsResolver(const core::dbus::Bus::Ptr& bus);

    /// @brief resolve_credentials resolves the credentials of the application that sent msg.
    void resolve_credentials(const core::dbus::Message::Ptr& msg,
                             const std::function<void(const Optional<RequestVerifier::Credentials>&)>& then) override;

private:
    core::dbus::Bus::Ptr bus;
};
}
}
}

#endif // BIOMETRYD_DBUS_SKELETON_DAEMON_CREDENTIALS_RESOLVER_H_

