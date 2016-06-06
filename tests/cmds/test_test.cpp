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

#include <biometry/cmds/test.h>

#include <biometry/device_registrar.h>
#include <biometry/devices/plugin/enumerator.h>

#include <core/posix/fork.h>

#include <gtest/gtest.h>

#include "did_finish_successfully.h"

#include <fstream>

namespace cli = biometry::util::cli;

namespace
{
struct NullEnumerator : public biometry::devices::plugin::Enumerator
{
    std::size_t enumerate(const Functor& ) const override
    {
        return 0;
    }
};

struct CmdTest : public ::testing::Test
{
    biometry::DeviceRegistrar device_registrar{NullEnumerator{}};
};
}

TEST_F(CmdTest, throws_configuration_invalid_if_device_is_not_given)
{
    {std::remove("invalid.json"); std::ofstream out{"invalid.json"}; out << R"_({ "device": {}})_" << std::endl;}

    biometry::cmds::Test test;
    EXPECT_THROW(test.run(cli::Command::Context{std::cin, std::cout, {"--config=invalid.json"}}), biometry::cmds::Test::ConfigurationInvalid);
}

TEST_F(CmdTest, throws_could_not_instantiate_device_for_unknown_id)
{
    {std::remove("invalid.json"); std::ofstream out{"invalid.json"}; out << R"_({ "device": {"id": "unknown"}})_" << std::endl;}

    biometry::cmds::Test test;
    EXPECT_THROW(test.run(cli::Command::Context{std::cin, std::cout, {"--config=invalid.json"}}), biometry::cmds::Test::CouldNotInstiantiateDevice);
}

TEST_F(CmdTest, runs_successfully_for_dummy_device)
{
    auto d = []()
    {
        {std::remove("dummy.json"); std::ofstream out{"dummy.json"}; out << R"_({"device": {"id": "Dummy"}})_" << std::endl;}

        biometry::cmds::Test test;
        EXPECT_EQ(EXIT_SUCCESS, test.run(cli::Command::Context{std::cin, std::cout, {"--config=dummy.json"}}));
        return testing::Test::HasFailure() ?
                    core::posix::exit::Status::failure :
                    core::posix::exit::Status::success;
    };

    auto cp = core::posix::fork(d, core::posix::StandardStream::stdin); cp.cin() << 'y' << std::endl;
    EXPECT_TRUE(testing::did_finish_successfully(
                    cp.wait_for(
                        core::posix::wait::Flags::untraced)));
}
