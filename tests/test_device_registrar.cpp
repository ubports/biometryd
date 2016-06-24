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

#include <biometry/devices/dummy.h>
#include <biometry/devices/plugin/device.h>
#include <biometry/devices/plugin/enumerator.h>

#include "config.h"

#include <gmock/gmock.h>

namespace
{
struct MockPluginEnumerator : public biometry::devices::plugin::Enumerator
{
    MOCK_CONST_METHOD1(enumerate, std::size_t(const Functor&));
};
}

TEST(DeviceRegistrar, cleans_out_device_registry)
{
    {
        biometry::DeviceRegistrar dr{biometry::devices::plugin::DirectoryEnumerator{{testing::runtime_dir()}}};
        EXPECT_GE(biometry::device_registry().size(), 2);
    }
    EXPECT_EQ(0, biometry::device_registry().size());
}

TEST(DeviceRegistrar, calls_into_enumerator)
{
    using namespace ::testing;

    MockPluginEnumerator enumerator;
    EXPECT_CALL(enumerator, enumerate(_)).Times(1).WillOnce(Return(0));

    biometry::DeviceRegistrar dr{enumerator};
}

TEST(DeviceRegistrar, adds_dummy_device)
{
    biometry::DeviceRegistrar dr{biometry::devices::plugin::DirectoryEnumerator{{testing::runtime_dir()}}};
    EXPECT_EQ(1, biometry::device_registry().count(biometry::devices::Dummy::id));
}

TEST(DeviceRegistrar, adds_plugin_device)
{
    biometry::DeviceRegistrar dr{biometry::devices::plugin::DirectoryEnumerator{{testing::runtime_dir()}}};
    EXPECT_EQ(1, biometry::device_registry().count(biometry::devices::plugin::id));
}

