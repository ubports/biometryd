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

#include <util/json.hpp>

#include <boost/format.hpp>
#include <boost/optional.hpp>

#include <fstream>
#include <string>
#include <tuple>

util::Configuration::Node::Node(const Variant &value)
    : value_{value}
{
}

const util::Variant& util::Configuration::Node::value() const
{
    return value_;
}

util::Configuration::Node& util::Configuration::Node::value(const Variant& value)
{
    value_ = value;
    return *this;
}

const util::Configuration::Children& util::Configuration::Node::children() const
{
    return children_;
}

util::Configuration::Children& util::Configuration::Node::children()
{
    return children_;
}

util::Configuration::Node& util::Configuration::Node::operator[](const std::string& name)
{
    return children_[name];
}

const util::Configuration::Children& util::Configuration::children() const
{
    return children_;
}

util::Configuration::Children& util::Configuration::children()
{
    return children_;
}

util::Configuration::Node& util::Configuration::operator[](const std::string& name)
{
    return children_[name];
}

namespace json = nlohmann;

namespace
{
boost::optional<util::Configuration::Node> build_from_json(const json::json& j)
{
    if (j.is_boolean())
        return util::Configuration::Node{util::Variant::b(j)};

    if (j.is_number_integer())
        return util::Configuration::Node{util::Variant::i(j)};

    if (j.is_number_float())
        return util::Configuration::Node{util::Variant::d(j)};

    if (j.is_string())
        return util::Configuration::Node{util::Variant::s(j)};


    if (j.is_array())
    {
        util::Configuration::Node result;
        for (std::size_t i = 0; i < j.size(); i++)
            if (auto n = build_from_json(j.at(i)))
                result[std::to_string(i)] = *n;

        return result;
    }

    if (j.is_object())
    {
        util::Configuration::Node result;
        for(auto it = j.begin(); it != j.end(); ++it)
            if (auto n = build_from_json(*it))
                result[it.key()] = *n;

        return result;
    }

    return boost::optional<util::Configuration::Node>{};
}
}

util::JsonConfigurationBuilder::JsonConfigurationBuilder(const boost::filesystem::path& path) : path{path}
{
}

util::Configuration util::JsonConfigurationBuilder::build_configuration()
{
    std::ifstream in{path.string().c_str()};
    if (not in)
        throw std::runtime_error
        {
            (boost::format{"Failed to open %1% for reading."} % path.string()).str()
        };

    util::Configuration result;

    auto j = json::json::parse(in);
    for (auto it = j.begin(); it != j.end(); ++it)
        if (auto n = build_from_json(*it))
            result.children()[it.key()] = *n;

    return result;
}
