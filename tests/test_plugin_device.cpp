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

#include <biometry/devices/plugin/device.h>
#include <util/dynamic_library.h>

#include <gmock/gmock.h>

#include <fstream>
#include <system_error>

#include "config.h"

namespace
{
struct MockDynamicLibraryApi : public util::DynamicLibrary::Api
{
    MOCK_CONST_METHOD1(open, util::DynamicLibrary::Handle(const boost::filesystem::path& path));
    MOCK_CONST_METHOD1(close, void(const util::DynamicLibrary::Handle&));
    MOCK_CONST_METHOD2(sym, util::DynamicLibrary::Symbol(const util::DynamicLibrary::Handle&, const std::string& symbol));
    MOCK_CONST_METHOD0(error, std::string());
};

struct MockPluginLoader : public biometry::devices::plugin::Loader
{
    MOCK_CONST_METHOD2(
            verify_and_load,
            std::shared_ptr<biometry::Device> (const std::shared_ptr<util::DynamicLibrary::Api>&, const boost::filesystem::path&));
};
}

TEST(PluginDeviceLoad, calls_into_loader)
{
    using namespace testing;

    std::shared_ptr<util::DynamicLibrary::Api> api = std::make_shared<NiceMock<MockDynamicLibraryApi>>();
    const boost::filesystem::path path{"/tmp/does/not/exist/module.so"};

    MockPluginLoader loader;
    EXPECT_CALL(loader, verify_and_load(api, path)).Times(1).WillOnce(Return(std::shared_ptr<biometry::Device>{}));

    biometry::devices::plugin::load(api, path, loader);
}

TEST(NonVerifyingLoader, can_load_plugin)
{
    const auto p = testing::runtime_dir() / "libbiometryd_devices_plugin_dl.so";

    biometry::devices::plugin::NonVerifyingLoader loader;
    EXPECT_NO_THROW(loader.verify_and_load(util::glibc::dl_api(), p));
}

TEST(ElfDescriptorLoader, can_load_from_plugin)
{
    const auto p = testing::runtime_dir() / "libbiometryd_devices_plugin_dl.so";
    biometry::devices::plugin::ElfDescriptorLoader loader;
    auto desc = loader.load_with_name(p, BIOMETRYD_DEVICES_PLUGIN_DESCRIPTOR_SECTION);

    EXPECT_STREQ("TestPlugin", desc.name);
    EXPECT_STREQ("Thomas Voß <thomas.voss@canonical.com>", desc.author);
    EXPECT_STREQ("Just a plugin for testing purposes", desc.description);
    EXPECT_EQ(0, desc.version.plugin.major);
    EXPECT_EQ(0, desc.version.plugin.minor);
    EXPECT_EQ(0, desc.version.plugin.patch);
}

TEST(ElfDescriptorLoader, throws_for_section_not_being_found)
{
    const auto p = testing::runtime_dir() / "libbiometryd_devices_plugin_dl.so";
    biometry::devices::plugin::ElfDescriptorLoader loader;
    EXPECT_THROW(loader.load_with_name(p, "DoesNotExist"), biometry::devices::plugin::ElfDescriptorLoader::NoSuchSection);
}

TEST(ElfDescriptorLoader, throws_when_trying_to_load_non_existing_file)
{
    std::remove("test.txt");
    biometry::devices::plugin::ElfDescriptorLoader loader;
    EXPECT_THROW(loader.load_with_name("test.txt", "DoesNotExist"), std::system_error);
}

TEST(ElfDescriptorLoader, throws_when_trying_to_load_non_elf_object)
{
    std::remove("test.txt");
    {std::ofstream out("test.txt"); out << "test";}
    biometry::devices::plugin::ElfDescriptorLoader loader;
    EXPECT_THROW(loader.load_with_name("test.txt", "DoesNotExist"), std::runtime_error);
}
