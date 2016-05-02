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

#ifndef BIOMETRYD_DBUS_SERVICE_H_
#define BIOMETRYD_DBUS_SERVICE_H_

#include <biometry/visibility.h>

#include <memory>

namespace biometry
{
/// @cond
class Service;
/// @endcond
namespace dbus
{
struct BIOMETRY_DLL_PUBLIC Service
{
    // We just use Service as a structuring entity.
    Service() = delete;

    /// @brief create_stub returns an implementation of biometry::Service
    /// transparently accessing a remote instance exposed via dbus.
    /// @throws std::runtime_error in case of issues.
    static std::shared_ptr<biometry::Service> create_stub();
};
}
}

#endif // BIOMETRYD_DBUS_SERVICE_H_
