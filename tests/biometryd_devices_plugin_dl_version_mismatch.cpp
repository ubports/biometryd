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

#include <biometry/devices/plugin/interface.h>

#include "mock_device.h"

/// [Defining the create function]
BIOMETRYD_DEVICES_PLUGIN_CREATE
{
    return new testing::MockDevice();
}
/// [Defining the create function]

/// [Defining the destroy function]
BIOMETRYD_DEVICES_PLUGIN_DESTROY
{
    delete d;
}
/// [Defining the destroy function]

/// Manually describe the plugin, incresing the major version by 1 to indicate an ABI break.
biometry::devices::plugin::Descriptor biometryd_devices_plugin_descriptor __attribute((section(BIOMETRYD_DEVICES_PLUGIN_DESCRIPTOR_SECTION))) =
    { "name", "author", "description", {{biometry::build::version_major+1, biometry::build::version_minor, biometry::build::version_patch}, {0, 0, 0}}};
