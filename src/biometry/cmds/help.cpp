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

#include <biometry/cmds/help.h>

biometry::cmds::Help::Help(const CommandEnumerator& enumerator) : enumerator{enumerator}
{
}

biometry::Daemon::Command::Info biometry::cmds::Help::info() const
{
    return Info
    {
        Name{"help"},
        Usage{"help"},
        Description{"print a help message and exit"},
        {}
    };
}

int biometry::cmds::Help::run()
{
    std::cout << "Usage: biometryd [COMMAND] \n"
                 "\n"
                 "biometryd mediates access to biometric devices. \n"
                 "\n"
                 "Commands:\n";

    enumerator([](const Command::Ptr& command)
    {
        std::cout << "  " << command->info().name << "\t" << command->info().description << std::endl;
    });

    return 0;
}
