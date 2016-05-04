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

#include <biometry/util/configuration.h>

#include <boost/format.hpp>
#include <boost/optional.hpp>

#include <fstream>
#include <string>
#include <tuple>

namespace
{
const biometry::util::Configuration::Node& null()
{
    static const biometry::util::Configuration::Node instance;
    return instance;
}
}
biometry::util::Configuration::Node::Node(const Variant &value)
    : value_{value}
{
}

const biometry::Variant& biometry::util::Configuration::Node::value() const
{
    return value_;
}

biometry::util::Configuration::Node& biometry::util::Configuration::Node::value(const Variant& value)
{
    value_ = value;
    return *this;
}

const biometry::util::Configuration::Children& biometry::util::Configuration::Node::children() const
{
    return children_;
}

biometry::util::Configuration::Children& biometry::util::Configuration::Node::children()
{
    return children_;
}

biometry::util::Configuration::Node& biometry::util::Configuration::Node::operator[](const std::string& name)
{
    return children_[name];
}

const biometry::util::Configuration::Children& biometry::util::Configuration::children() const
{
    return children_;
}

biometry::util::Configuration::Children& biometry::util::Configuration::children()
{
    return children_;
}

biometry::util::Configuration::Node& biometry::util::Configuration::operator[](const std::string& name)
{
    return children_[name];
}

const biometry::util::Configuration::Node& biometry::util::Configuration::operator[](const std::string& name) const
{
    auto it = children_.find(name);
    if (it != children_.end())
        return it->second;

    return null();
}
