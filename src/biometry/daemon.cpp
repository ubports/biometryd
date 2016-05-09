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
#include <biometry/cmds/help.h>
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

namespace
{
std::multimap<cli::Command::Name, std::function<void(const std::string&)>>& notifiers()
{
    static std::multimap<cli::Command::Name, std::function<void(const std::string&)>> instance;
    return instance;
}
}

cli::Command::Flag::Flag(const Name& name, const Description& description)
    : name_{name},
      description_{description}
{
}

const cli::Command::Name& cli::Command::Flag::name() const
{
    return name_;
}

const cli::Command::Description& cli::Command::Flag::description() const
{
    return description_;
}

biometry::Daemon::Daemon() : device_registrar{biometry::devices::plugin::DirectoryEnumerator{Configuration::default_plugin_directory()}}
{
    install_command(std::make_shared<cmds::Enroll>());
    install_command(std::make_shared<cmds::Identify>());
    install_command(std::make_shared<cmds::ListDevices>());
    install_command(std::make_shared<cmds::Run>());
    install_command(std::make_shared<cmds::Version>());

    help = std::make_shared<cmds::Help>([this](const cmds::Help::Enumerator& enumerator)
    {
        for (const auto& pair : cmds)
            enumerator(pair.second);
    });
}

int biometry::Daemon::run(const std::vector<std::string>& args)
{
    po::positional_options_description pdesc;
    pdesc.add("command", 1);

    po::options_description desc("Options");
    desc.add_options()
            ("command", po::value<std::string>()->default_value(help->info().name), "the command to be executed");

    for (const auto& pair : cmds)
    {
        po::options_description cd(pair.first);
        for (auto flag : pair.second->info().flags)
        {
            // Depending on the name of the flag, we only ever install it once
            // to the options_description to avoid triggering an issue caused
            // by ambigious options. Instead, we relay the notify method to inform multiple
            // flags of the same name that a new value is available.
            if (notifiers().count(flag->name()) == 0)
            {
                auto v = po::value<std::string>()->notifier([flag](const std::string& s)
                {
                    // Iterate over all known identifiers.
                    auto range = notifiers().equal_range(flag->name());
                    while (range.first != range.second)
                    {
                        (range.first->second)(s);
                        ++range.first;
                    }
                });

                cd.add_options()(flag->name().as_string().c_str(), v, flag->description().as_string().c_str());
            }

            notifiers().insert(std::make_pair(flag->name(), [flag](const std::string& s) { flag->notify(s); }));
        }
        desc.add(cd);
    }

    try
    {
        po::variables_map vm;
        po::store(po::command_line_parser(args).options(desc).positional(pdesc).run(), vm);
        po::notify(vm);

        auto command = vm["command"].as<std::string>();

        if (command == "help")
            return help->run();

        return cmds[command]->run();
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;
        return help->run();
    }
}

void biometry::Daemon::install_command(const cli::Command::Ptr& command)
{
    cmds[command->info().name] = command;
}
