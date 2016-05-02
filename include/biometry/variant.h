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

#ifndef BIOMETRYD_VARIANT_H_
#define BIOMETRYD_VARIANT_H_

#include <biometry/geometry.h>
#include <biometry/visibility.h>

#include <cstdint>

#include <iosfwd>
#include <memory>
#include <string>
#include <vector>

namespace biometry
{
class BIOMETRY_DLL_PUBLIC Variant
{
public:
    /// @brief None models the unset value.
    struct None {};

    enum class Type
    {
        none,
        boolean,
        integer,
        floating_point,
        rectangle,
        string,
        blob,
        vector
    };

    static Variant b(bool value);
    static Variant i(std::int64_t value);
    static Variant d(double value);
    static Variant r(const biometry::Rectangle& value);
    static Variant s(const std::string& value);
    static Variant bl(const std::vector<std::uint8_t>& value);
    static Variant v(const std::vector<Variant>& value);

    Variant();
    Variant(const Variant&);
    explicit Variant(bool b);
    explicit Variant(std::int64_t i);
    explicit Variant(double d);
    explicit Variant(const biometry::Rectangle& value);
    explicit Variant(const std::string& s);
    explicit Variant(const std::vector<std::uint8_t>& b);
    explicit Variant(const std::vector<Variant>& b);
    ~Variant();

    Variant& operator=(const Variant&);
    bool operator==(const Variant&) const;

    std::ostream& print(std::ostream&) const;

    Type type() const;

    const bool& boolean() const;
    void boolean(bool value);

    std::int64_t integer() const;
    void integer(std::int64_t value);

    double floating_point() const;
    void floating_point(double value);

    const biometry::Rectangle& rectangle() const;
    void rectangle(const biometry::Rectangle& rectangle);

    const std::string& string() const;
    void string(const std::string& value);

    const std::vector<std::uint8_t>& blob() const;
    void blob(const std::vector<std::uint8_t>& value);

    const std::vector<Variant>& vector() const;
    void vector(const std::vector<Variant>& vector);

private:
    struct Private;
    std::unique_ptr<Private> p;
};
BIOMETRY_DLL_PUBLIC bool operator==(const Variant::None&, const Variant::None&);
BIOMETRY_DLL_PUBLIC std::ostream& operator<<(std::ostream& out, const Variant::None&);
BIOMETRY_DLL_PUBLIC std::ostream& operator<<(std::ostream& out, const Variant& v);
}
#endif // UTIL_VARIANT_H_
