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

#include <biometry/daemon.h>

#include <biometry/devices/plugin/enumerator.h>

#include <biometry/cmds/enroll.h>
#include <biometry/cmds/identify.h>
#include <biometry/cmds/list_devices.h>
#include <biometry/cmds/run.h>
#include <biometry/cmds/version.h>

#include <boost/program_options.hpp>

#include <functional>
#include <iostream>
#include <map>

namespace cli = biometry::util::cli;
namespace po = boost::program_options;

biometry::Daemon::Daemon()
    : device_registrar{biometry::devices::plugin::DirectoryEnumerator{Configuration::default_plugin_directory()}},
      cmd{cli::Name{"biometryd"}, cli::Usage{"biometryd"}, cli::Description{"biometryd"}}
{
    cmd.command(std::make_shared<cmds::Enroll>())
       .command(std::make_shared<cmds::Identify>())
       .command(std::make_shared<cmds::ListDevices>())
       .command(std::make_shared<cmds::Run>())
       .command(std::make_shared<cmds::Version>());
}

int biometry::Daemon::run(const std::vector<std::string>& args)
{
    return cmd.run({std::cin, std::cout, args});
}
