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

#ifndef BIOMETRYD_DEVICES_PLUGIN_INTERFACE_H_
#define BIOMETRYD_DEVICES_PLUGIN_INTERFACE_H_

#include <biometry/version.h>

#define BIOMETRYD_DEVICES_PLUGIN_DESCRIPTOR_SECTION "BIOMETRYD_DEVICES_PLUGIN_DESCRIPTOR"

namespace biometry
{
class Device;

namespace devices
{
namespace plugin
{
static constexpr const std::uint32_t name_length        = 256;
static constexpr const std::uint32_t author_length      = 256;
static constexpr const std::uint32_t description_length = 256;

struct Version
{
    std::uint32_t major, minor, patch;
};

struct Descriptor
{
    const char name[name_length];
    const char author[author_length];
    const char description[description_length];

    struct
    {
        Version host;
        Version plugin;
    } const version;
};
}
}
}

/// @brief BiometrydPluginDeviceCreate defines the function used to create biometry::Device instances.
typedef biometry::Device*   (*BiometrydPluginDeviceCreate)    ();
/// @brief BiometrydPluginDeviceDestroy defines the function used to destroy biometry::Device instances.
typedef void                (*BiometrydPluginDeviceDestroy)   (biometry::Device*);

/// @snippet tests/biometryd_devices_plugin_dl.cpp Describing the plugin
#define BIOMETRYD_DEVICES_PLUGIN_DESCRIBE(name, author, description, major, minor, patch) \
    biometry::devices::plugin::Descriptor biometryd_devices_plugin_descriptor __attribute((section(BIOMETRYD_DEVICES_PLUGIN_DESCRIPTOR_SECTION))) = \
        { name, author, description, {{biometry::build::version_major, biometry::build::version_minor, biometry::build::version_patch}, {major, minor, patch}}};

/// @brief Starts the implementation of the create function exposed from a dynamic library.
///
/// @snippet tests/biometryd_devices_plugin_dl.cpp Defining the create function
#define BIOMETRYD_DEVICES_PLUGIN_CREATE \
    extern "C" __attribute__ ((visibility ("default"))) biometry::Device* CreateBiometrydDevice()

/// @snippet tests/biometryd_devices_plugin_dl.cpp Defining the destroy function
/// @brief Starts implementation of the destroy function exposed from a dynamic library.
///
/// @code
/// @endcode
#define BIOMETRYD_DEVICES_PLUGIN_DESTROY \
    extern "C" __attribute__ ((visibility ("default"))) void DestroyBiometrydDevice(biometry::Device* d)

/// @brief Tags the symbol name of the create function.
#define BIOMETRYD_DEVICES_PLUGIN_CREATE_SYMBOL_NAME "CreateBiometrydDevice"
/// @brief Tags the symbol name of the destroy function.
#define BIOMETRYD_DEVICES_PLUGIN_DESTROY_SYMBOL_NAME "DestroyBiometrydDevice"



#endif // BIOMETRYD_DEVICES_PLUGIN_INTERFACE_H_
