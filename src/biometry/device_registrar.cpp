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

#include <biometry/device_registrar.h>

#include <biometry/device_registry.h>

#include <biometry/devices/android.h>
#include <biometry/devices/dummy.h>
#include <biometry/devices/plugin/device.h>
#include <biometry/devices/plugin/enumerator.h>

namespace plugin = biometry::devices::plugin;

biometry::DeviceRegistrar::DeviceRegistrar(const biometry::devices::plugin::Enumerator& enumerator)
{
    biometry::device_registry()[biometry::devices::Dummy::id] = biometry::devices::Dummy::make_descriptor();
    biometry::device_registry()[biometry::devices::plugin::id] = biometry::devices::plugin::make_descriptor();
    biometry::device_registry()[biometry::devices::android::id] = biometry::devices::android::make_descriptor();

    enumerator.enumerate([](const biometry::Device::Descriptor::Ptr& desc)
    {
        biometry::device_registry()[desc->name()] = desc;
    });
}

biometry::DeviceRegistrar::~DeviceRegistrar()
{
    biometry::device_registry().clear();
}
