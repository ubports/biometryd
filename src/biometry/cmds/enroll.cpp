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
#include <biometry/device_registry.h>
#include <biometry/template_store.h>
#include <biometry/tracing_operation_observer.h>

#include <biometry/cmds/enroll.h>

#include <biometry/util/configuration.h>
#include <biometry/util/json_configuration_builder.h>
#include <biometry/util/streaming_configuration_builder.h>

#include <fstream>
#include <iostream>
#include <memory>

biometry::cmds::Enroll::Enroll()
    : device{biometry::make_command_flag<std::string>(Name{"device"}, Description{"The device to enroll to"})},
      config{std::make_shared<TypedFlag<boost::filesystem::path>>(Command::Name{"config"}, Command::Description{"The daemon configuration"})},
      user{biometry::make_command_flag<biometry::User>(Name{"user"}, Description{"The user to enroll for"})}
{
    user->value(biometry::User::current());
}

biometry::Daemon::Command::Info biometry::cmds::Enroll::info() const
{
    return Info
    {
        Name{"enroll"},
        Usage{"enroll"},
        Description{"enrolls a new template to a device"},
        {device, user}
    };
}

int biometry::cmds::Enroll::run()
{
    if (not device->value())
    {
        std::cout << "You must specify a device for enrolling a template" << std::endl;
        return EXIT_FAILURE;
    }

    using StreamingJsonConfigurationBuilder = util::StreamingConfigurationBuilder<util::JsonConfigurationBuilder>;
    StreamingJsonConfigurationBuilder builder
    {
         config->value() ?
             StreamingJsonConfigurationBuilder::make_streamer(config->value().get()) :
             StreamingJsonConfigurationBuilder::make_streamer(std::cin)
    };

    auto descriptor = biometry::device_registry().at(*device->value());
    auto device = descriptor->create(builder.build_configuration());

    auto op = device->template_store().enroll(biometry::Application{"system"}, biometry::User::current());

    std::cout << "Starting template enrollment for " << user->value() << " to " << descriptor->name() << std::endl;

    op->start_with_observer(std::make_shared<TracingObserver<biometry::TemplateStore::Enrollment>>());

    return 0;
}
