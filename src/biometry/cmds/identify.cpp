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

#include <biometry/application.h>
#include <biometry/device.h>
#include <biometry/device_registry.h>
#include <biometry/identifier.h>
#include <biometry/reason.h>
#include <biometry/tracing_operation_observer.h>

#include <biometry/cmds/identify.h>

#include <iostream>
#include <memory>


biometry::cmds::Identify::Identify()
    : device{biometry::make_command_flag<std::string>(Name{"device"}, Description{"The device to enroll to"})}
{
}

biometry::Daemon::Command::Info biometry::cmds::Identify::info() const
{
    return Info
    {
        Name{"identify"},
        Usage{"identify"},
        Description{"tries to identify the user holding the device"},
        {device}
    };
}

int biometry::cmds::Identify::run()
{
    if (not device->value())
    {
        std::cout << "You must specify a device for identification" << std::endl;
        return EXIT_FAILURE;
    }

    auto descriptor = biometry::device_registry().at(*device->value());
    auto device = descriptor->create();

    auto op = device->identifier().identify_user(biometry::Application{"system"}, biometry::Reason{"requested by cli"});

    std::cout << "Starting identification using device " << descriptor->name() << std::endl;

    op->start_with_observer(std::make_shared<TracingObserver<biometry::Identification>>());

    return 0;
}
