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

#include <biometry/cmds/run.h>

#include <gmock/gmock.h>

#include <iostream>

namespace
{
struct MockPropertyStore : public biometry::util::PropertyStore
{
    MOCK_CONST_METHOD1(get, std::string(const std::string&));
};
}

TEST(CmdRun, configuration_oracle_queries_device_property)
{
    using namespace ::testing;

    MockPropertyStore mock;
    EXPECT_CALL(mock, get("ro.product.device")).Times(1).WillOnce(Return("turbo"));
    biometry::cmds::Run::ConfigurationOracle oracle;

    EXPECT_EQ("meizu::FingerprintReader", oracle.make_an_educated_guess(mock));
}

TEST(CmdRun, queries_configuration_oracle_if_no_config_given)
{
    using namespace ::testing;

    auto mock = std::make_shared<MockPropertyStore>();
    EXPECT_CALL(*mock, get("ro.product.device")).Times(1).WillOnce(Return("turbo"));

    biometry::cmds::Run run{mock, biometry::cmds::Run::system_bus_factory()};
    EXPECT_EQ(EXIT_SUCCESS, run.run(biometry::util::cli::Command::Context{std::cin, std::cout, {}}));
}

TEST(CmdRun, android_property_store_throws_if_getprop_missing)
{
    biometry::util::AndroidPropertyStore store;
    EXPECT_ANY_THROW(store.get("ro.build.product"));
}
