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

#ifndef BIOMETRYD_DEVICE_REGISTRY_H_
#define BIOMETRYD_DEVICE_REGISTRY_H_

#include <biometry/device.h>
#include <biometry/visibility.h>

#include <unordered_map>

namespace biometry
{
typedef std::unordered_map<Device::Id, Device::Descriptor::Ptr> DeviceRegistry;

/// @brief device_registry returns the process-wide registry of known devices:
BIOMETRY_DLL_PUBLIC DeviceRegistry& device_registry();
}

#endif // BIOMETRYD_DEVICE_REGISTRY_H_
