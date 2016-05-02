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

#include <biometry/percent.h>

#include <gtest/gtest.h>

TEST(Percent, construction_throws_for_value_out_of_range)
{
    EXPECT_ANY_THROW(biometry::Percent::from_raw_value(2.));
}

TEST(Percent, construction_succeeds_for_value_in_range)
{
    EXPECT_NO_THROW(biometry::Percent::from_raw_value(.5));
}

TEST(Percent, value_can_be_retrieved)
{
    auto percent = biometry::Percent::from_raw_value(.43);
    EXPECT_DOUBLE_EQ(.43, *percent);
}

TEST(Percent, can_be_copy_constructed)
{
    biometry::Percent copy{biometry::Percent::from_raw_value(.43)};
    EXPECT_DOUBLE_EQ(.43, *copy);
}

TEST(Percent, can_be_assigned)
{
    biometry::Percent copy = biometry::Percent::from_raw_value(.43);
    EXPECT_DOUBLE_EQ(.43, *copy);
}
