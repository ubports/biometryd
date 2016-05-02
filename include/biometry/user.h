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

#ifndef BIOMETRY_USER_H_
#define BIOMETRY_USER_H_

#include <biometry/visibility.h>

#include <iosfwd>

#include <sys/types.h>

namespace biometry
{
/// @brief User models
struct BIOMETRY_DLL_PUBLIC User
{
    /// @brief root returns an instance of User corresponding to the system's root user.
    static const User& root();
    /// @brief current returns an instance of User corresponding to the currently active user.
    static const User& current();

    uid_t id; ///< @brief The numeric id of the user.
};

/// @brief operator< returns true iff lhs's id is smaller than rhs's id.
BIOMETRY_DLL_PUBLIC bool operator<(const User& lhs, const User& rhs);
/// @brief operator== returns true iff lhs's id equals rhs's id.
BIOMETRY_DLL_PUBLIC bool operator==(const User& lhs, const User& rhs);
/// @brief operator<< inserts user into out.
BIOMETRY_DLL_PUBLIC std::ostream& operator<<(std::ostream& out, const User& user);
/// @brief operator>> extracts user from in.
BIOMETRY_DLL_PUBLIC std::istream& operator>>(std::istream& in, User& user);
}

#endif // BIOMETRY_USER_H_
