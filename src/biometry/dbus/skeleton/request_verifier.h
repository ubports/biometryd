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

#ifndef BIOMETRYD_DBUS_SKELETON_REQUEST_VERIFIER_H_
#define BIOMETRYD_DBUS_SKELETON_REQUEST_VERIFIER_H_

#include <biometry/application.h>
#include <biometry/do_not_copy_or_move.h>
#include <biometry/user.h>

#include <iosfwd>

namespace biometry
{
namespace dbus
{
namespace skeleton
{
/// @brief RequestVerifier is the top-level point of entry to the world
/// of verifying incoming requests for different components.
///
/// Most importantly, it defines the struct Credentials bundling together
/// information about an incoming request.
class RequestVerifier : public DoNotCopyOrMove
{
public:
    /// @brief Credentials bundles an app and a user.
    struct Credentials
    {
        Application app; ///< The id of the requesting component.
        User user;       ///< The user under which the requesting component is running.
    };
};

/// @brief operator== returns true iff lhs and rhs compare equal per component.
bool operator==(const RequestVerifier::Credentials& lhs, const RequestVerifier::Credentials& rhs);
/// @brief operator<< inserts credentials into out.
std::ostream& operator<<(std::ostream& out, const RequestVerifier::Credentials& credentials);
}
}
}

#endif // BIOMETRYD_DBUS_SKELETON_REQUEST_VERIFIER_H_

