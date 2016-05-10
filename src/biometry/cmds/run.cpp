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

#include <biometry/cmds/run.h>

#include <biometry/device_registry.h>
#include <biometry/dispatching_service.h>
#include <biometry/runtime.h>
#include <biometry/dbus/skeleton/service.h>

#include <biometry/util/configuration.h>
#include <biometry/util/json_configuration_builder.h>
#include <biometry/util/streaming_configuration_builder.h>

#include <core/dbus/bus.h>
#include <core/dbus/asio/executor.h>

#include <core/posix/signal.h>

#include <fstream>

namespace cli = biometry::util::cli;

biometry::cmds::Run::BusFactory biometry::cmds::Run::system_bus_factory()
{
    return []()
    {
        return std::make_shared<core::dbus::Bus>(core::dbus::WellKnownBus::system);
    };
}


biometry::cmds::Run::Run(const BusFactory& bus_factory)
    : CommandWithFlagsAndAction{cli::Name{"run"}, cli::Usage{"run"}, cli::Description{"run the daemon"}},
      bus_factory{bus_factory}
{
    flag(cli::make_flag(cli::Name{"config"}, cli::Description{"The daemon configuration"}, config));
    action([this](const cli::Command::Context&)
    {
        auto trap = core::posix::trap_signals_for_all_subsequent_threads({core::posix::Signal::sig_term});
        trap->signal_raised().connect([trap](const core::posix::Signal&)
        {
            trap->stop();
        });

        using StreamingJsonConfigurationBuilder = util::StreamingConfigurationBuilder<util::JsonConfigurationBuilder>;
        StreamingJsonConfigurationBuilder builder
        {
            config ?
                StreamingJsonConfigurationBuilder::make_streamer(config.get()) :
                StreamingJsonConfigurationBuilder::make_streamer(std::cin)
        };
        auto configuration = builder.build_configuration();

        auto default_device = configuration["defaultDevice"];
        auto default_device_descriptor = device_registry().at(default_device["id"].value().string());

        util::Configuration device_config; device_config["config"] = default_device["config"];
        auto device = default_device_descriptor->create(device_config);

        auto runtime = Runtime::create();
        auto impl = std::make_shared<biometry::DispatchingService>(biometry::util::create_dispatcher_for_runtime(runtime), device);

        runtime->start();

        auto bus = this->bus_factory();
        bus->install_executor(core::dbus::asio::make_executor(bus, runtime->service()));

        auto skeleton = biometry::dbus::skeleton::Service::create_for_bus(bus, impl);

        trap->run();

        bus->stop();
        runtime->stop();

        return EXIT_SUCCESS;
    });
}
