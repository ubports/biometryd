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

#ifndef BIOMETRYD_DEVICE_REGISTRAR_H_
#define BIOMETRYD_DEVICE_REGISTRAR_H_

#include <biometry/device.h>
#include <biometry/visibility.h>

namespace biometry
{
namespace devices
{
namespace plugin
{
/// @cond
class Enumerator;
/// @endcond
}
}
/// @brief DeviceRegistrar makes devices known to the device registry.
///
/// A DeviceRegistrar adds Device::Descriptor instances to the registry for both:
///   - builtin devices
///   - plugin devices that place their modules into a well-known location
struct BIOMETRY_DLL_PUBLIC DeviceRegistrar
{
    /// @brief DeviceRegistrar initializes the registry for builtin devices and the plugins reported by enumerator.
    DeviceRegistrar(const biometry::devices::plugin::Enumerator& enumerator);
    /// @brief DeviceRegistrar cleans out the device registry.
    ~DeviceRegistrar();
};
}

#endif // BIOMETRYD_DEVICE_REGISTRAR_H_
