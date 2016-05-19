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

#ifndef BIOMETRYD_CMDS_TEST_H_
#define BIOMETRYD_CMDS_TEST_H_

#include <biometry/device.h>
#include <biometry/optional.h>
#include <biometry/user.h>
#include <biometry/visibility.h>

#include <biometry/util/cli.h>

#include <boost/filesystem.hpp>

#include <functional>
#include <iostream>
#include <memory>

namespace biometry
{
namespace cmds
{
// biometryd test --help
// NAME:
//     test - executes runtime tests for a device
//
// USAGE:
//     test [command options] [arguments...]
//
// OPTIONS:
//     --config          configuration file for the test
//     --user            The numeric user id for testing purposes
class BIOMETRY_DLL_PUBLIC Test : public util::cli::CommandWithFlagsAndAction
{
public:
    struct ConfigurationInvalid : public std::runtime_error
    {
        ConfigurationInvalid();
    };

    struct CouldNotInstiantiateDevice : public std::runtime_error
    {
        CouldNotInstiantiateDevice();
    };

    Test();

    int test_device(const User& user, const Command::Context& ctxt, const std::shared_ptr<Device>& device);
private:
    Optional<boost::filesystem::path> config;
    User user;
    std::uint32_t trials;
};
}
}

#endif // BIOMETRYD_CMDS_TEST_H_
