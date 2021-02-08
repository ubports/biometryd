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

#include <biometry/variant.h>

#include <boost/variant.hpp>

#include <iostream>

namespace
{
typedef typename boost::make_recursive_variant
<
    biometry::Variant::None,
    bool,
    std::int64_t,
    double,
    biometry::Rectangle,
    std::string,
    std::vector<std::uint8_t>,
    std::vector<biometry::Variant>
>::type Value;

struct Visitor : public boost::static_visitor<std::ostream&>
{
    Visitor(std::ostream& out) : out{out}
    {
    }

    template<typename T>
    std::ostream& operator()(const T& t)
    {
        return out << t;
    }

    std::ostream& operator()(const std::vector<std::uint8_t>& v)
    {
        return out << "@" << &v[0];
    }

    std::ostream& operator()(const std::vector<biometry::Variant>& v)
    {
        return out << "@" << &v[0];
    }

    std::ostream& out;
};
}

struct biometry::Variant::Private
{
    Value value;
};

biometry::Variant biometry::Variant::b(bool value)
{
    return Variant(value);
}

biometry::Variant biometry::Variant::i(std::int64_t value)
{
    return Variant{value};
}

biometry::Variant biometry::Variant::d(double value)
{
    return Variant{value};
}

biometry::Variant biometry::Variant::r(const biometry::Rectangle& value)
{
    return biometry::Variant{value};
}

biometry::Variant biometry::Variant::s(const std::string& value)
{
    return Variant{value};
}

biometry::Variant biometry::Variant::bl(const std::vector<std::uint8_t>& value)
{
    return Variant{value};
}

biometry::Variant biometry::Variant::v(const std::vector<biometry::Variant>& value)
{
    return Variant{value};
}

biometry::Variant::Variant(const Variant& rhs) : p{new Private{rhs.p->value}}
{
}

biometry::Variant::Variant() : p{new Private{None{}}}
{
}

biometry::Variant::Variant(bool b) : p{new Private{b}}
{
}

biometry::Variant::Variant(std::int64_t i) : p{new Private{i}}
{
}

biometry::Variant::Variant(double fp) : p{new Private{fp}}
{
}

biometry::Variant::Variant(const biometry::Rectangle& r) : p{new Private{r}}
{
}

biometry::Variant::Variant(const std::string& s) : p{new Private{s}}
{
}

biometry::Variant::Variant(const std::vector<std::uint8_t>& b) : p{new Private{b}}
{
}

biometry::Variant::Variant(const std::vector<biometry::Variant>& v) : p{new Private{v}}
{
}

biometry::Variant::~Variant()
{
}

biometry::Variant& biometry::Variant::operator=(const biometry::Variant& rhs)
{
    *p = *rhs.p;
    return *this;
}

bool biometry::Variant::operator==(const biometry::Variant& rhs) const
{
    return p->value == rhs.p->value;
}

std::ostream& biometry::Variant::print(std::ostream& out) const
{
    Visitor v{out};
    return p->value.apply_visitor(v);
}

biometry::Variant::Type biometry::Variant::type() const
{
    return static_cast<Type>(p->value.which());
}

const bool& biometry::Variant::boolean() const
{
    return boost::get<bool>(p->value);
}

void biometry::Variant::boolean(bool value)
{
    p->value = value;
}

std::int64_t biometry::Variant::integer() const
{
    return boost::get<std::int64_t>(p->value);
}

void biometry::Variant::integer(std::int64_t value)
{
    p->value = value;
}

double biometry::Variant::floating_point() const
{
    return boost::get<double>(p->value);
}

void biometry::Variant::floating_point(double value)
{
    p->value = value;
}

const biometry::Rectangle& biometry::Variant::rectangle() const
{
    return boost::get<biometry::Rectangle>(p->value);
}

void biometry::Variant::rectangle(const biometry::Rectangle& value)
{
    p->value = value;
}

const std::string& biometry::Variant::string() const
{
    return boost::get<std::string>(p->value);
}

void biometry::Variant::string(const std::string& value)
{
    p->value = value;
}

const std::vector<std::uint8_t>& biometry::Variant::blob() const
{
    return boost::get<std::vector<std::uint8_t>>(p->value);
}

void biometry::Variant::blob(const std::vector<std::uint8_t>& value)
{
    p->value = value;
}

const std::vector<biometry::Variant>& biometry::Variant::vector() const
{
    return boost::get<std::vector<Variant>>(p->value);
}

void biometry::Variant::vector(const std::vector<biometry::Variant>& value)
{
    p->value = value;
}

bool biometry::operator==(const biometry::Variant::None&, const biometry::Variant::None&)
{
    return true;
}

std::ostream& biometry::operator<<(std::ostream& out, const Variant::None&)
{
    return out << "none";
}

std::ostream& biometry::operator<<(std::ostream& out, const Variant& v)
{
    return v.print(out);
}
