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

#include <util/configuration.h>

#include <gtest/gtest.h>

#include <fstream>

TEST(Configuration, default_constructor_yields_valid_configuration)
{
    util::Configuration config;
}

TEST(ConfigurationNode, setting_value_of_node_works)
{
    util::Configuration config;
    config.children()["test"] = util::Configuration::Node{util::Variant::i(42)};
    ASSERT_EQ(1, config.children().count("test"));
    ASSERT_EQ(42, config.children().at("test").value().integer());
}

TEST(Variant, constructors_yield_correct_type_and_value)
{
    {const bool rv = true; util::Variant v{rv}; EXPECT_EQ(util::Variant::Type::boolean, v.type()); EXPECT_EQ(rv, v.boolean());}
    {const std::int64_t rv = 42; util::Variant v{rv}; EXPECT_EQ(util::Variant::Type::integer, v.type()); EXPECT_EQ(rv, v.integer());}
    {const double rv = 42.f; util::Variant v{rv}; EXPECT_EQ(util::Variant::Type::floating_point, v.type()); EXPECT_EQ(rv, v.floating_point());}
    {const std::string rv = "42"; util::Variant v{rv}; EXPECT_EQ(util::Variant::Type::string, v.type()); EXPECT_EQ(rv, v.string());}
    {const std::vector<std::uint8_t> rv = {4, 2}; util::Variant v{rv}; EXPECT_EQ(util::Variant::Type::blob, v.type()); EXPECT_EQ(rv, v.blob());}
}

TEST(Variant, named_constructors_yield_correct_type_and_value)
{
    {const bool rv = true; auto v = util::Variant::b(rv); EXPECT_EQ(util::Variant::Type::boolean, v.type()); EXPECT_EQ(rv, v.boolean());}
    {const std::int64_t rv = 42; auto v = util::Variant::i(rv); EXPECT_EQ(util::Variant::Type::integer, v.type()); EXPECT_EQ(rv, v.integer());}
    {const double rv = 42.f; auto v = util::Variant::d(rv); EXPECT_EQ(util::Variant::Type::floating_point, v.type()); EXPECT_EQ(rv, v.floating_point());}
    {const std::string rv = "42"; auto v = util::Variant::s(rv); EXPECT_EQ(util::Variant::Type::string, v.type()); EXPECT_EQ(rv, v.string());}
    {const std::vector<std::uint8_t> rv = {4, 2}; auto v = util::Variant::bl(rv); EXPECT_EQ(util::Variant::Type::blob, v.type()); EXPECT_EQ(rv, v.blob());}
}

TEST(Variant, stream_insertion_operator_works)
{
    util::Variant v; std::cout << v << std::endl;
}

TEST(JsonConfigurationBuilder, works_for_valid_json)
{
    auto json = R"_({
                "devices" :
                [
                    {
                        "device.id" : "meizu::FingerprintReader",
                        "device.config": {
                            "templateId.store": "SqliteTemplateIdStore",
                            "templateId.store.config": {
                                "db.path": "/var/lib/biometryd/ids.db"
                            }
                        }
                    },
                    {
                        "device.id" : "biometryd::Plugin",
                        "device.config" : {
                            "plugin.path": "/usr/lib/x86_64/biometryd/plugins/",
                            "plugin.config": {

                            }
                        }
                    }
                ]
            })_";

    {
        std::remove("test.json");
        std::ofstream out{"test.json"};
        out << json;
    }

    util::JsonConfigurationBuilder builder{"test.json"};
    auto config = builder.build_configuration();

    EXPECT_EQ(1, config.children().count("devices"));
    EXPECT_EQ(2, config["devices"].children().size());

    auto devices = config["devices"];

    auto _0 = devices["0"];
    auto _1 = devices["1"];

    EXPECT_EQ("meizu::FingerprintReader", _0["device.id"].value().string());
    EXPECT_EQ("biometryd::Plugin", _1["device.id"].value().string());
}
