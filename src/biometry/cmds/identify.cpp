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

#include <biometry/util/configuration.h>
#include <biometry/util/json_configuration_builder.h>
#include <biometry/util/streaming_configuration_builder.h>

#include <fstream>
#include <iostream>
#include <memory>

namespace cli = biometry::util::cli;

biometry::cmds::Identify::Identify()
    : CommandWithFlagsAndAction{cli::Name{"identify"}, cli::Usage{"identify"}, cli::Description{"tries to identify the user holding the device"}}
{
    flag(cli::make_flag(cli::Name{"device"}, cli::Description{"The device to enroll to"}, device));
    flag(cli::make_flag(cli::Name{"config"}, cli::Description{"The daemon configuration"}, config));
    action([this](const cli::Command::Context& ctxt)
    {
        if (device.empty())
        {
            ctxt.cout << "You must specify a device for identification" << std::endl;
            return EXIT_FAILURE;
        }

        using StreamingJsonConfigurationBuilder = util::StreamingConfigurationBuilder<util::JsonConfigurationBuilder>;
        StreamingJsonConfigurationBuilder builder
        {
            config ?
                StreamingJsonConfigurationBuilder::make_streamer(config.get()) :
                StreamingJsonConfigurationBuilder::make_streamer(std::cin)
        };

        auto descriptor = biometry::device_registry().at(device);
        auto device = descriptor->create(builder.build_configuration());

        auto op = device->identifier().identify_user(biometry::Application::system(), biometry::Reason{"requested by cli"});

        ctxt.cout << "Starting identification using device " << descriptor->name() << std::endl;

        op->start_with_observer(std::make_shared<TracingObserver<biometry::Identification>>());

        return 0;
    });
}
