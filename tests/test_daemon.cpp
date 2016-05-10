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

#include <gtest/gtest.h>

#include <core/dbus/fixture.h>
#include <core/posix/fork.h>

#include <did_finish_successfully.h>

#include <fstream>
#include <thread>

namespace cli = biometry::util::cli;

namespace
{
struct Daemon : core::dbus::testing::Fixture
{
};
}

TEST(SizeConstrainedString, throws_on_construction_for_string_too_long)
{
    EXPECT_ANY_THROW(cli::SizeConstrainedString<0>{"test"});
}

TEST(SizeConstrainedString, construction_succeeds_for_string_of_correct_length)
{
    EXPECT_NO_THROW(cli::SizeConstrainedString<10>{"test"});
}

TEST(SizeConstrainedString, operator_lt_works)
{
    cli::SizeConstrainedString<10> a{"a"};
    cli::SizeConstrainedString<10> b{"b"};

    EXPECT_TRUE(a < b);
    EXPECT_FALSE(b < a);
}

TEST(SizeConstrainedString, stores_string_content_passed_on_construction)
{
    cli::SizeConstrainedString<20> a{"a"};
    EXPECT_EQ("a", a.as_string());
}

TEST(TypedFlag, stores_name_and_desc_passed_on_construction)
{
    cli::Name name{"42"};
    cli::Description desc{"43"};
    cli::TypedFlag<int> flag{name, desc};
    EXPECT_EQ(name, flag.name());
    EXPECT_EQ(desc, flag.description());
}

TEST(TypedFlag, parses_string_on_notify_and_sets_value)
{
    cli::Name name{"42"};
    cli::Description desc{"43"};
    cli::TypedFlag<int> flag{name, desc};
    EXPECT_FALSE(flag.value().is_initialized());
    flag.notify("42");
    EXPECT_TRUE(flag.value().is_initialized());
    EXPECT_EQ(42, *flag.value());
}

TEST_F(Daemon, invoking_help_command_succeeds)
{
    auto d = []()
    {
        biometry::Daemon daemon;
        EXPECT_EQ(EXIT_FAILURE, daemon.run({"help"}));
        return testing::Test::HasFailure() ?
                    core::posix::exit::Status::failure :
                    core::posix::exit::Status::success;
    };

    EXPECT_TRUE(testing::did_exit(
                    core::posix::fork(d, core::posix::StandardStream::empty)
                        .wait_for(core::posix::wait::Flags::untraced)));
}

TEST_F(Daemon, invoking_version_command_succeeds)
{
    auto d = []()
    {
        biometry::Daemon daemon;
        EXPECT_EQ(EXIT_SUCCESS, daemon.run({"version"}));

        return testing::Test::HasFailure() ?
                    core::posix::exit::Status::failure :
                    core::posix::exit::Status::success;
    };

    EXPECT_TRUE(testing::did_finish_successfully(
                    core::posix::fork(d, core::posix::StandardStream::empty)
                        .wait_for(core::posix::wait::Flags::untraced)));
}

TEST_F(Daemon, invoking_list_devices_command_succeeds)
{
    auto d = []()
    {
        biometry::Daemon daemon;
        EXPECT_EQ(EXIT_SUCCESS, daemon.run({"list-devices"}));

        return testing::Test::HasFailure() ?
                    core::posix::exit::Status::failure :
                    core::posix::exit::Status::success;
    };

    EXPECT_TRUE(testing::did_finish_successfully(
                    core::posix::fork(d, core::posix::StandardStream::empty)
                        .wait_for(core::posix::wait::Flags::untraced)));
}

TEST_F(Daemon, invoking_run_succeeds)
{
    auto json = R"_(
        {
            "defaultDevice":
            {
                "id": "Dummy",
                "config":{}
            }
        })_";

    {
        std::remove("test.json"); std::ofstream out{"test.json"}; out << json;
    }

    auto d = []()
    {
        biometry::Daemon daemon;
        auto rc = daemon.run(
        {
            "run", "--config=test.json"
        });

        return rc == EXIT_SUCCESS ? core::posix::exit::Status::success : core::posix::exit::Status::failure;
    };

    auto dp = core::posix::fork(d, core::posix::StandardStream::empty);

    std::this_thread::sleep_for(std::chrono::milliseconds{500});

    dp.send_signal_or_throw(core::posix::Signal::sig_term);
    EXPECT_TRUE(testing::did_finish_successfully(dp.wait_for(core::posix::wait::Flags::untraced)));
}
