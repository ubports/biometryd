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

#ifndef BIOMETRYD_APPLICATION_H_
#define BIOMETRYD_APPLICATION_H_

#include <biometry/visibility.h>

#include <string>

namespace biometry
{
/// @brief Application describes an application instance.
class BIOMETRY_DLL_PUBLIC Application
{
public:
    /// @brief system returns an application instance referring to the overall system.
    static Application system();

    /// @brief Application initializes the instance with system().
    Application();

    /// @brief Application initializes an instance with the given string.
    explicit Application(const std::string& s);

    /// @brief as_string returns a string representation of this instance.
    const std::string& as_string() const;

    /// @brief operator std::string returns a string representation of this instance.
    operator std::string() const;

private:
    std::string value;
};

/// @brief operator== returns true if lhs has the same name as rhs.
BIOMETRY_DLL_PUBLIC bool operator==(const Application& lhs, const Application& rhs);
}

#endif // BIOMETRYD_APPLICATION_H_
