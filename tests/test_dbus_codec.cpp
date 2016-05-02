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

#include <biometry/runtime.h>
#include <biometry/dbus/codec.h>

#include <core/dbus/fixture.h>
#include <core/dbus/macros.h>
#include <core/dbus/message.h>
#include <core/dbus/object.h>

#include <core/posix/fork.h>
#include <core/posix/signal.h>

#include <core/dbus/asio/executor.h>

#include <gmock/gmock.h>

#include "did_finish_successfully.h"
#include "echo_service.h"

namespace
{
struct VerifyingEchoService : public testing::EchoService
{
    struct Reference
    {
        static biometry::Application application()
        {
            return biometry::Application{"JustSomeDummyApplication"};
        }

        static biometry::Progress progress()
        {
            biometry::Progress result;
            result.percent = biometry::Percent::from_raw_value(0.42);
            return result;
        }

        static biometry::Point point()
        {
            return biometry::Point{42, 42};
        }

        static biometry::Rectangle rectangle()
        {
            return {point(), point()};
        }

        static biometry::Reason reason()
        {
            return biometry::Reason{"because we are testing"};
        }

        static biometry::User user()
        {
            return biometry::User::current();
        }

        static biometry::Variant::None none()
        {
            return biometry::Variant::None{};
        }

        static biometry::Variant variant()
        {
            return biometry::Variant::v(
            {
                biometry::Variant::b(true),
                biometry::Variant::i(42),
                biometry::Variant::d(42.),
                biometry::Variant::r(Reference::rectangle()),
                biometry::Variant::s("42"),
                biometry::Variant::v(
                {
                    biometry::Variant::b(true),
                    biometry::Variant::i(42),
                    biometry::Variant::d(42.),
                    biometry::Variant::r(Reference::rectangle()),
                    biometry::Variant::s("42")
                })
            });
        }

        static biometry::Void void_()
        {
            return biometry::Void{};
        }
    };

    void application(const biometry::Application& value)
    {
        EXPECT_EQ(Reference::application(), value);
    }

    void progress(const biometry::Progress& value)
    {
        EXPECT_EQ(Reference::progress(), value);
    }

    void point(const biometry::Point& value)
    {
        EXPECT_EQ(Reference::point(), value);
    }

    void rectangle(const biometry::Rectangle& value)
    {
        EXPECT_EQ(Reference::rectangle(), value);
    }

    void reason(const biometry::Reason& value)
    {
        EXPECT_EQ(Reference::reason(), value);
    }

    void user(const biometry::User& value)
    {
        EXPECT_EQ(Reference::user(), value);
    }

    void none(const biometry::Variant::None& value)
    {
        EXPECT_EQ(Reference::none(), value);
    }

    void variant(const biometry::Variant& value)
    {
        EXPECT_EQ(Reference::variant(), value);
    }

    void void_(const biometry::Void& value)
    {
        EXPECT_EQ(Reference::void_(), value);
    }
};
}

struct DbusCodec : public core::dbus::testing::Fixture
{
};

TEST_F(DbusCodec, encoding_and_decoding_works_for_relevant_types)
{
    auto sk = [this]()
    {
        auto trap = core::posix::trap_signals_for_all_subsequent_threads({core::posix::Signal::sig_term});
        trap->signal_raised().connect([trap](core::posix::Signal)
        {
            trap->stop();
        });

        auto bus = session_bus();
        bus->install_executor(core::dbus::asio::make_executor(bus));

        std::thread t{[bus]() { bus->run(); }};

        auto impl = std::make_shared<VerifyingEchoService>();
        auto sk = std::make_shared<testing::SkeletonEchoService>(bus,
                    core::dbus::Service::add_service(bus, testing::EchoService::name())
                        ->add_object_for_path(testing::EchoService::path()),
                    impl);

        trap->run();

        bus->stop();

        if (t.joinable())
            t.join();

        return Test::HasFailure() ? core::posix::exit::Status::failure : core::posix::exit::Status::success;
    };

    auto st = [this]()
    {
        auto bus = session_bus();
        bus->install_executor(core::dbus::asio::make_executor(bus));

        std::thread t{[bus]() { bus->run(); }};

        auto st = std::make_shared<testing::StubEchoService>(core::dbus::Service::use_service(bus, testing::EchoService::name())->object_for_path(testing::EchoService::path()));

        EXPECT_NO_THROW(st->application(VerifyingEchoService::Reference::application()));
        EXPECT_NO_THROW(st->progress(VerifyingEchoService::Reference::progress()));
        EXPECT_NO_THROW(st->point(VerifyingEchoService::Reference::point()));
        EXPECT_NO_THROW(st->rectangle(VerifyingEchoService::Reference::rectangle()));
        EXPECT_NO_THROW(st->reason(VerifyingEchoService::Reference::reason()));
        EXPECT_NO_THROW(st->user(VerifyingEchoService::Reference::user()));
        EXPECT_NO_THROW(st->none(VerifyingEchoService::Reference::none()));
        EXPECT_NO_THROW(st->variant(VerifyingEchoService::Reference::variant()));
        EXPECT_NO_THROW(st->void_(VerifyingEchoService::Reference::void_()));

        bus->stop(); if (t.joinable()) t.join();

        return Test::HasFailure() ? core::posix::exit::Status::failure : core::posix::exit::Status::success;
    };

    auto skp = core::posix::fork(sk, core::posix::StandardStream::empty);
    std::this_thread::sleep_for(std::chrono::milliseconds{500});
    auto stp = core::posix::fork(st, core::posix::StandardStream::empty);

    EXPECT_TRUE(testing::did_finish_successfully(stp.wait_for(core::posix::wait::Flags::untraced)));
    EXPECT_NO_THROW(skp.send_signal_or_throw(core::posix::Signal::sig_term));
    EXPECT_TRUE(testing::did_finish_successfully(skp.wait_for(core::posix::wait::Flags::untraced)));
}
