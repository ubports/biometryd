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

#ifndef BIOMETRYD_DBUS_SKELETON_CREDENTIALS_RESOLVER_H_
#define BIOMETRYD_DBUS_SKELETON_CREDENTIALS_RESOLVER_H_

#include <biometry/do_not_copy_or_move.h>
#include <biometry/optional.h>
#include <biometry/dbus/skeleton/request_verifier.h>

#include <core/dbus/message.h>

#include <functional>

namespace biometry
{
namespace dbus
{
namespace skeleton
{
/// @brief CredentialsResolver resolves incoming messages to RequestVerifier::Credentials.
class CredentialsResolver : public DoNotCopyOrMove
{
public:
    /// @brief resolve_credentials resolves the credentials of the application that sent msg.
    virtual void resolve_credentials(const core::dbus::Message::Ptr& msg,
                                     const std::function<void(const Optional<RequestVerifier::Credentials>&)>& then) = 0;
};
}
}
}

#endif // BIOMETRYD_DBUS_SKELETON_CREDENTIALS_RESOLVER_H_

