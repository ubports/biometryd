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

#include <biometry/util/json_configuration_builder.h>

#include <biometry/util/json.hpp>

#include <boost/format.hpp>
#include <boost/optional.hpp>

#include <fstream>

namespace json = nlohmann;

namespace
{
boost::optional<biometry::util::Configuration::Node> build_from_json(const json::json& j)
{
    if (j.is_boolean())
        return biometry::util::Configuration::Node{biometry::Variant::b(j)};

    if (j.is_number_integer())
        return biometry::util::Configuration::Node{biometry::Variant::i(j)};

    if (j.is_number_float())
        return biometry::util::Configuration::Node{biometry::Variant::d(j)};

    if (j.is_string())
        return biometry::util::Configuration::Node{biometry::Variant::s(j)};


    if (j.is_array())
    {
        biometry::util::Configuration::Node result;
        for (std::size_t i = 0; i < j.size(); i++)
            if (auto n = build_from_json(j.at(i)))
                result[std::to_string(i)] = *n;

        return result;
    }

    if (j.is_object())
    {
        biometry::util::Configuration::Node result;
        for(auto it = j.begin(); it != j.end(); ++it)
            if (auto n = build_from_json(*it))
                result[it.key()] = *n;

        return result;
    }

    return boost::optional<biometry::util::Configuration::Node>{};
}
}

biometry::util::JsonConfigurationBuilder::JsonConfigurationBuilder(std::istream& in) : in{in}
{
    if (not in) throw std::runtime_error
    {
        "Input stream is not available for reading."
    };
}

biometry::util::Configuration biometry::util::JsonConfigurationBuilder::build_configuration()
{
    util::Configuration result;

    auto j = json::json::parse(in);
    for (auto it = j.begin(); it != j.end(); ++it)
        if (auto n = build_from_json(*it))
            result.children()[it.key()] = *n;

    return result;
}
