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

#include <biometry/geometry.h>

#include <gtest/gtest.h>

TEST(Point, equality_operator_works)
{
    biometry::Point p1{42, 43}; biometry::Point p2(p1);
    EXPECT_EQ(p2, p1);
    biometry::Point p3{43, 42};
    EXPECT_NE(p3, p1);
}

TEST(Point, stream_insertion_works)
{
    biometry::Point p1{42, 42};
    std::stringstream ss; ss << p1;
    EXPECT_EQ("(42,42)", ss.str());
}

TEST(Rectangle, equality_operator_works)
{
    biometry::Point p1{42, 43}; biometry::Point p2(p1);
    biometry::Rectangle r1{p1, p2}; biometry::Rectangle r2(r1);
    EXPECT_EQ(r2, r1);
    biometry::Point p3{43, 42};
    biometry::Rectangle r3{p1, p3};
    EXPECT_NE(r3, r1);
}

TEST(Rectangle, stream_insertion_works)
{
    biometry::Point p1{42, 43}; biometry::Point p2(p1);
    biometry::Rectangle r1{p1, p2};
    std::stringstream ss; ss << r1;
    EXPECT_EQ("((42,43),(42,43))", ss.str());
}


