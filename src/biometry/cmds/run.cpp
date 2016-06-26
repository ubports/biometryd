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
#include <unordered_map>

namespace cli = biometry::util::cli;

namespace
{
const std::unordered_map<std::string, biometry::Device::Id>& device_id_lut()
{
    static const std::unordered_map<std::string, std::string>& instance
    {
        {"turbo", "meizu::FingerprintReader"}
    };

    return instance;
}

std::shared_ptr<biometry::Device> device_from_config(const boost::filesystem::path& config_file)
{
    using StreamingJsonConfigurationBuilder = biometry::util::StreamingConfigurationBuilder<biometry::util::JsonConfigurationBuilder>;
    StreamingJsonConfigurationBuilder builder{StreamingJsonConfigurationBuilder::make_streamer(config_file)};
    auto configuration = builder.build_configuration();

    auto default_device = configuration["defaultDevice"];
    biometry::util::Configuration device_config; device_config["config"] = default_device["config"];
    auto default_device_descriptor = biometry::device_registry().at(default_device[std::string("id")].value().string());

    return default_device_descriptor->create(device_config);
}

std::shared_ptr<biometry::Device> device_from_oracle(const biometry::util::PropertyStore& property_store)
{
    auto id = biometry::cmds::Run::ConfigurationOracle{}.make_an_educated_guess(property_store);
    auto default_device_descriptor = biometry::device_registry().at(id);
    return default_device_descriptor->create({});
}

std::shared_ptr<biometry::Device> create_default_device(const biometry::Optional<boost::filesystem::path>& config_file, const biometry::util::PropertyStore& property_store)
{
    return config_file ? device_from_config(*config_file) : device_from_oracle(property_store);
}
}

biometry::Device::Id biometry::cmds::Run::ConfigurationOracle::make_an_educated_guess(const biometry::util::PropertyStore& property_store) const
{
    auto value = property_store.get("ro.product.device");

    if (value.empty())
        throw std::runtime_error{"Could not identify device"};

    if (device_id_lut().count(value) == 0)
        throw std::runtime_error{"Unknown device: " + value};

    return device_id_lut().at(value);
}

biometry::cmds::Run::BusFactory biometry::cmds::Run::system_bus_factory()
{
    return []()
    {
        return std::make_shared<core::dbus::Bus>(core::dbus::WellKnownBus::system);
    };
}

biometry::cmds::Run::Run(const std::shared_ptr<biometry::util::PropertyStore>& property_store, const BusFactory& bus_factory)
    : CommandWithFlagsAndAction{cli::Name{"run"}, cli::Usage{"run"}, cli::Description{"run the daemon"}},
      bus_factory{bus_factory},
      property_store{property_store}
{
    flag(cli::make_flag(cli::Name{"config"}, cli::Description{"The daemon configuration"}, config));
    action([this](const cli::Command::Context& ctxt)
    {
        auto trap = core::posix::trap_signals_for_all_subsequent_threads({core::posix::Signal::sig_term});
        trap->signal_raised().connect([trap](const core::posix::Signal&)
        {
            trap->stop();
        });

        std::shared_ptr<biometry::Device> device;
        try
        {
            device = create_default_device(config, *Run::property_store);
        }
        catch (...)
        {
            ctxt.cout << "Failed to instantiate device." << std::endl;
            return EXIT_FAILURE;
        }

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
