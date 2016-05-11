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

#include <biometry/cmds/test.h>

#include <biometry/application.h>
#include <biometry/device_registry.h>
#include <biometry/identifier.h>
#include <biometry/reason.h>
#include <biometry/template_store.h>
#include <biometry/tracing_operation_observer.h>
#include <biometry/user.h>

#include <biometry/util/configuration.h>
#include <biometry/util/json_configuration_builder.h>
#include <biometry/util/streaming_configuration_builder.h>

namespace cli = biometry::util::cli;

biometry::cmds::Test::ConfigurationInvalid::ConfigurationInvalid()
    : std::runtime_error{"Configuration is invalid"}
{
}


biometry::cmds::Test::CouldNotInstiantiateDevice::CouldNotInstiantiateDevice()
    : std::runtime_error{"Could not instantiate device"}
{
}

biometry::cmds::Test::Test()
    : CommandWithFlagsAndAction{cli::Name{"test"}, cli::Usage{"executes runtime tests for a device"}, cli::Description{"executes runtime tests for a device"}},
      user{biometry::User::current()}
{
    flag(cli::make_flag(cli::Name{"config"}, cli::Description{"configuration file for the test"}, config));
    flag(cli::make_flag(cli::Name{"user"}, cli::Description{"The numeric user id for testing purposes"}, user));
    action([this](const cli::Command::Context& ctxt)
    {
        if (not config) throw cli::Command::FlagsMissing{};

        using StreamingJsonConfigurationBuilder = util::StreamingConfigurationBuilder<util::JsonConfigurationBuilder>;
        StreamingJsonConfigurationBuilder builder
        {
            config ?
                StreamingJsonConfigurationBuilder::make_streamer(config.get()) :
                StreamingJsonConfigurationBuilder::make_streamer(std::cin)
        };

        const auto configuration = builder.build_configuration();

        static const auto throw_configuration_invalid = [](){ std::throw_with_nested(ConfigurationInvalid{});};

        const auto id = configuration
                ("device",  throw_configuration_invalid)
                ("id",      throw_configuration_invalid);
        util::Configuration config; config["config"] = configuration
                ("device",  throw_configuration_invalid)
                ["config"];

        if (id)
        {
            Device::Descriptor::Ptr desc;
            std::shared_ptr<Device> device;
            try
            {
                device = (desc = device_registry().at(id.value().string()))->create(config);
            } catch(...) { std::throw_with_nested(CouldNotInstiantiateDevice{});}

            ctxt.cout
                    << "We are about to execute a test run for a biometric device." << std::endl
                    << "Please note that we are executing the test in a production" << std::endl
                    << "environment and you should consider the test to be harmful to the" << std::endl
                    << "device configuration for device with:" << std::endl
                    << "  Name:        " << desc->name() << std::endl
                    << "  Description: " << desc->description() << std::endl
                    << "  Author:      " << desc->author() << std::endl
                    << "and configuration:" << std::endl
                    << "  User:        " << user << std::endl
                    << "  Config:      " << *Test::config << std::endl
                    << "Would you really like to proceed (y/n)?";

            static constexpr const char yes{'y'}; char answer; ctxt.cin >> answer;
            return answer == yes ? test_device(user, ctxt, device) : EXIT_FAILURE;
        } else {throw std::runtime_error{"Invalid configuration."};}

        return EXIT_FAILURE;
    });
}

int biometry::cmds::Test::test_device(const User& user, const cli::Command::Context& ctxt, const std::shared_ptr<Device>& device)
{
    ctxt.cout << std::endl;
    ctxt.cout << "Clearing template store:" << std::endl;
    device->template_store().clear(biometry::Application::system(), user)->start_with_observer(
        std::make_shared<biometry::TracingObserver<biometry::TemplateStore::Clearance>>(2, ctxt.cout));

    ctxt.cout << "Enrolling template:" << std::endl;
    device->template_store().enroll(biometry::Application::system(), user)->start_with_observer(
        std::make_shared<biometry::TracingObserver<biometry::TemplateStore::Enrollment>>(2, ctxt.cout));

    ctxt.cout << "Querying template count:" << std::endl;
    device->template_store().size(biometry::Application::system(), user)->start_with_observer(
        std::make_shared<biometry::TracingObserver<biometry::TemplateStore::SizeQuery>>(2, ctxt.cout));

    ctxt.cout << "Identifying user:" << std::endl;
    device->identifier().identify_user(biometry::Application::system(), biometry::Reason{"testing"})->start_with_observer(
        std::make_shared<biometry::TracingObserver<biometry::Identification>>(2, ctxt.cout));

    return EXIT_SUCCESS;
}
