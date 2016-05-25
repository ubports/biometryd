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

#include <biometry/cmds/config.h>

#include <biometry/daemon.h>

#include <gtest/gtest.h>

#include <boost/algorithm/string.hpp>

namespace cli = biometry::util::cli;

TEST(CmdConfig, returns_correct_result_for_default_plugin_directory)
{
    std::stringstream ss; ss << biometry::cmds::Config::Flag::default_plugin_directory;

    std::stringstream cout;
    biometry::cmds::Config config;
    EXPECT_EQ(EXIT_SUCCESS, config.run(cli::Command::Context{std::cin, cout, {"--flag=" + ss.str()}}));
    EXPECT_EQ(biometry::Daemon::Configuration::default_plugin_directory().string(),
              boost::algorithm::trim_copy(cout.str()));
}

TEST(CmdConfig, throws_flags_missing_for_missing_flag)
{
    biometry::cmds::Config config;
    EXPECT_THROW(config.run(cli::Command::Context{std::cin, std::cout, {}}), cli::Command::FlagsMissing);
}

TEST(CmdConfig, throws_flags_with_invalid_value_for_invalid_flag)
{
    biometry::cmds::Config config;
    EXPECT_THROW(config.run(cli::Command::Context{std::cin, std::cout, {"--flag=invalid"}}), cli::Command::FlagsWithInvalidValue);
}
