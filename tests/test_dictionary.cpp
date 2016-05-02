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

#include <biometry/dictionary.h>

#include <gtest/gtest.h>

TEST(Dictionary, default_constructor_works)
{
    biometry::Dictionary{};
}

TEST(Dictionary, insertion_works)
{
    biometry::Dictionary dict;
    dict["test"] = biometry::Variant::i(42);
    EXPECT_TRUE(dict.count("test") > 0);
    EXPECT_EQ(42, dict.at("test").integer());
}

TEST(Dictionary, removal_works)
{
    biometry::Dictionary dict;
    dict["test"] = biometry::Variant::i(42);
    EXPECT_TRUE(dict.count("test") > 0);
    dict.erase("test");
    EXPECT_TRUE(dict.count("test") == 0);
}

TEST(Variant, constructors_yield_correct_type_and_value)
{
    {const bool rv = true; biometry::Variant v{rv}; EXPECT_EQ(biometry::Variant::Type::boolean, v.type()); EXPECT_EQ(rv, v.boolean());}
    {const std::int64_t rv = 42; biometry::Variant v{rv}; EXPECT_EQ(biometry::Variant::Type::integer, v.type()); EXPECT_EQ(rv, v.integer());}
    {const double rv = 42.f; biometry::Variant v{rv}; EXPECT_EQ(biometry::Variant::Type::floating_point, v.type()); EXPECT_EQ(rv, v.floating_point());}
    {const biometry::Rectangle rv = {biometry::Point{42, 42}, biometry::Point{42, 42}}; biometry::Variant v{rv}; EXPECT_EQ(biometry::Variant::Type::rectangle, v.type()); EXPECT_EQ(rv, v.rectangle());}
    {const std::string rv = "42"; biometry::Variant v{rv}; EXPECT_EQ(biometry::Variant::Type::string, v.type()); EXPECT_EQ(rv, v.string());}
    {const std::vector<std::uint8_t> rv = {4, 2}; biometry::Variant v{rv}; EXPECT_EQ(biometry::Variant::Type::blob, v.type()); EXPECT_EQ(rv, v.blob());}
    {const std::vector<biometry::Variant> rv = {biometry::Variant(std::int64_t(4)), biometry::Variant("2")}; biometry::Variant v{rv}; EXPECT_EQ(biometry::Variant::Type::vector, v.type()); EXPECT_EQ(rv, v.vector());}
}

TEST(Variant, named_constructors_yield_correct_type_and_value)
{
    {const bool rv = true; auto v = biometry::Variant::b(rv); EXPECT_EQ(biometry::Variant::Type::boolean, v.type()); EXPECT_EQ(rv, v.boolean());}
    {const std::int64_t rv = 42; auto v = biometry::Variant::i(rv); EXPECT_EQ(biometry::Variant::Type::integer, v.type()); EXPECT_EQ(rv, v.integer());}
    {const double rv = 42.f; auto v = biometry::Variant::d(rv); EXPECT_EQ(biometry::Variant::Type::floating_point, v.type()); EXPECT_EQ(rv, v.floating_point());}
    {const std::string rv = "42"; auto v = biometry::Variant::s(rv); EXPECT_EQ(biometry::Variant::Type::string, v.type()); EXPECT_EQ(rv, v.string());}
    {const biometry::Rectangle rv = {biometry::Point{42, 42}, biometry::Point{42, 42}}; auto v = biometry::Variant::r(rv); EXPECT_EQ(biometry::Variant::Type::rectangle, v.type()); EXPECT_EQ(rv, v.rectangle());}
    {const std::vector<std::uint8_t> rv = {4, 2}; auto v = biometry::Variant::bl(rv); EXPECT_EQ(biometry::Variant::Type::blob, v.type()); EXPECT_EQ(rv, v.blob());}
    {const std::vector<biometry::Variant> rv = {biometry::Variant::i(4), biometry::Variant::s("2")}; biometry::Variant v{rv}; EXPECT_EQ(biometry::Variant::Type::vector, v.type()); EXPECT_EQ(rv, v.vector());}
}
