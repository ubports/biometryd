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

#include <util/variant.h>

namespace
{
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

    std::ostream& out;
};
}
util::Variant util::Variant::b(bool value)
{
    return Variant{value};
}

util::Variant util::Variant::i(std::int64_t value)
{
    return Variant{value};
}

util::Variant util::Variant::d(double value)
{
    return Variant{value};
}

util::Variant util::Variant::s(const std::string& value)
{
    return Variant{value};
}

util::Variant util::Variant::bl(const std::vector<std::uint8_t>& value)
{
    return Variant{value};
}

util::Variant::Variant() : value_{None{}}
{
}

util::Variant::Variant(bool b) : value_{b}
{
}

util::Variant::Variant(std::int64_t i) : value_{i}
{
}

util::Variant::Variant(double d) : value_{d}
{
}

util::Variant::Variant(const std::string& s) : value_{s}
{
}

util::Variant::Variant(const std::vector<std::uint8_t>& b) : value_{b}
{
}

std::ostream& util::Variant::print(std::ostream& out) const
{
    Visitor v{out};
    return value_.apply_visitor(v);
}

util::Variant::Type util::Variant::type() const
{
    return static_cast<Type>(value_.which());
}

const bool& util::Variant::boolean() const
{
    return boost::get<bool>(value_);
}

void util::Variant::boolean(bool value)
{
    value_ = value;
}

std::int64_t util::Variant::integer() const
{
    return boost::get<std::int64_t>(value_);
}

void util::Variant::integer(std::int64_t value)
{
    value_ = value;
}

double util::Variant::floating_point() const
{
    return boost::get<double>(value_);
}

void util::Variant::floating_point(double value)
{
    value_ = value;
}

const std::string& util::Variant::string() const
{
    return boost::get<std::string>(value_);
}

void util::Variant::string(const std::string& value)
{
    value_ = value;
}

const std::vector<std::uint8_t>& util::Variant::blob() const
{
    return boost::get<std::vector<std::uint8_t>>(value_);
}

void util::Variant::blob(const std::vector<std::uint8_t>& value)
{
    value_ = value;
}

std::ostream& util::operator<<(std::ostream& out, const Variant::None&)
{
    return out << "none";
}

std::ostream& util::operator<<(std::ostream& out, const Variant& v)
{
    return v.print(out);
}
