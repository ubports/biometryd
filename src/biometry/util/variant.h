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

#ifndef UTIL_VARIANT_H_
#define UTIL_VARIANT_H_

#include <biometry/visibility.h>

#include <boost/variant.hpp>

#include <cstdint>

#include <iosfwd>
#include <string>
#include <vector>

namespace util
{
class BIOMETRY_DLL_PUBLIC Variant
{
public:

    /// @brief None models the unset value.
    struct None {};
    typedef boost::variant
    <
        None,
        bool,
        std::int64_t,
        double,
        std::string,
        std::vector<std::uint8_t>
    > Value;

    enum class Type
    {
        none,
        boolean,
        integer,
        floating_point,
        string,
        blob
    };

    static Variant b(bool value);
    static Variant i(std::int64_t value);
    static Variant d(double value);
    static Variant s(const std::string& value);
    static Variant bl(const std::vector<std::uint8_t>& value);

    Variant();
    explicit Variant(bool b);
    explicit Variant(std::int64_t i);
    explicit Variant(double d);
    explicit Variant(const std::string& s);
    explicit Variant(const std::vector<std::uint8_t>& b);

    std::ostream& print(std::ostream&) const;

    Type type() const;

    const bool& boolean() const;
    void boolean(bool value);

    std::int64_t integer() const;
    void integer(std::int64_t value);

    double floating_point() const;
    void floating_point(double value);

    const std::string& string() const;
    void string(const std::string& value);

    const std::vector<std::uint8_t>& blob() const;
    void blob(const std::vector<std::uint8_t>& value);

private:
    Value value_;
};

BIOMETRY_DLL_PUBLIC std::ostream& operator<<(std::ostream& out, const Variant::None&);
BIOMETRY_DLL_PUBLIC std::ostream& operator<<(std::ostream& out, const Variant& v);
}
#endif // UTIL_VARIANT_H_
