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

#ifndef BIOMETRYD_REASON_H_
#define BIOMETRYD_REASON_H_

#include <biometry/visibility.h>

#include <string>

namespace biometry
{
/// @brief Reason models the reason for requesting an identification or verification operation.
class BIOMETRY_DLL_PUBLIC Reason
{
public:
    /// @brief unknown returns a Reason instance referring to the unknown reason.
    static Reason unknown();

    /// @brief Reason initializes an instance to be equal to unknown().
    Reason();

    /// @brief Reason initializes an instance with the given string s.
    explicit Reason(const std::string& s);

    /// @brief as_string returns the string representation of this instance.
    const std::string& as_string() const;

    /// @brief operator std::string returns the string representation of this instance.
    operator std::string() const;

private:
    /// @cond
    std::string value;
    /// @endcond
};

/// @brief operator== returns true if lhs has the same name as rhs.
BIOMETRY_DLL_PUBLIC bool operator==(const Reason& lhs, const Reason& rhs);
}

#endif // BIOMETRYD_REASON_H_
