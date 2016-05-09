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

#ifndef BIOMETRYD_DEVICES_PLUGIN_DEVICE_H_
#define BIOMETRYD_DEVICES_PLUGIN_DEVICE_H_

#include <biometry/devices/forwarding.h>
#include <biometry/devices/plugin/loader.h>

#include <boost/filesystem.hpp>

namespace biometry
{
namespace devices
{
namespace plugin
{
/// @brief make_descriptor returns a descriptor instance describing a plugin device;
BIOMETRY_DLL_PUBLIC biometry::Device::Descriptor::Ptr make_descriptor();

/// @brief id is the unique name for registering the device type with the device registry.
static constexpr const char* id{"Plugin"};

/// @cond
using Device = biometry::devices::Forwarding;
/// @endcond

/// @brief load returns a biometry::Device implementation that has been loaded from a shared object located at path,
/// relying on api to open the library and resolve symbols.
BIOMETRY_DLL_PUBLIC std::shared_ptr<biometry::Device> load(const std::shared_ptr<util::DynamicLibrary::Api>& api, const boost::filesystem::path& path, const Loader& loader);
}
}
}

#endif // BIOMETRYD_DEVICES_PLUGIN_DEVICE_H_
