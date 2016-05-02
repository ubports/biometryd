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

#include <biometry/user.h>

#include <gtest/gtest.h>

#include <sstream>

TEST(User, inserts_correctly_into_output_stream)
{
    biometry::User user{0};
    std::stringstream ss; ss << user;
    EXPECT_EQ("User[0]", ss.str());
}

TEST(User, root_has_user_id_0)
{
    EXPECT_EQ(0, biometry::User::root().id);
}

TEST(User, current_user_returns_instance_with_correct_id)
{
    EXPECT_EQ(::getuid(), biometry::User::current().id);
}

TEST(User, less_than_operator_works)
{
    EXPECT_TRUE(biometry::User{1} < biometry::User{2});
    EXPECT_FALSE(biometry::User{2} < biometry::User{1});
}

TEST(User, set_of_users_works)
{
    std::set<biometry::User> users;
    users.insert(biometry::User::root());
    users.insert(biometry::User{42});

    EXPECT_EQ(2, users.size());
    EXPECT_TRUE(users.count(biometry::User::root()) > 0);
    EXPECT_TRUE(users.count(biometry::User{42}) > 0);
}
