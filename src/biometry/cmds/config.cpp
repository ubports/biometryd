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

#include <boost/bimap.hpp>

namespace cli = biometry::util::cli;

namespace
{
std::string enumerate_flags()
{
    std::stringstream ss;
    ss << biometry::cmds::Config::Flag::default_plugin_directory << ", "
       << biometry::cmds::Config::Flag::custom_plugin_directory;
    return ss.str();
}
}

biometry::cmds::Config::Config()
    : CommandWithFlagsAndAction{cli::Name{"config"}, cli::Usage{"queries configuration options of the daemon"}, cli::Description{"queries configuration options of the daemon"}}
{
    flag(cli::make_flag(cli::Name{"flag"}, cli::Description{"one of {" + enumerate_flags() + "}"}, flag_));

    action([this](const cli::Command::Context& ctxt)
    {
        if (not flag_) throw cli::Command::FlagsMissing{};

        switch (*flag_)
        {
        case Flag::default_plugin_directory:
            ctxt.cout << biometry::Daemon::Configuration::default_plugin_directory().string() << std::endl;
            break;
        case Flag::custom_plugin_directory:
            ctxt.cout << biometry::Daemon::Configuration::custom_plugin_directory().string() << std::endl;
            break;
        }

        return EXIT_SUCCESS;
    });
}

namespace
{
typedef boost::bimap<biometry::cmds::Config::Flag, std::string> Lut;
typedef Lut::value_type Entry;
const Lut& lut()
{
    static const auto entries =
    {
        Entry{biometry::cmds::Config::Flag::default_plugin_directory, "default_plugin_directory"},
        Entry{biometry::cmds::Config::Flag::custom_plugin_directory, "custom_plugin_directory"}
    };
    static const Lut instance{entries.begin(), entries.end()};
    return instance;
}
}

std::ostream& biometry::cmds::operator<<(std::ostream& out, biometry::cmds::Config::Flag flag)
{
    return out << lut().left.at(flag);
}

std::istream& biometry::cmds::operator>>(std::istream& in, biometry::cmds::Config::Flag& flag)
{
    std::string s; in >> s;

    if (lut().right.count(s) == 0)
        throw cli::Command::FlagsWithInvalidValue{};

    flag = lut().right.at(s);
    return in;
}

