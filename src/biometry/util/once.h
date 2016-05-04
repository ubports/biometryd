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

#ifndef BIOMETRY_UTIL_ONCE_H_
#define BIOMETRY_UTIL_ONCE_H_

#include <boost/optional.hpp>

#include <mutex>

namespace biometry
{
namespace util
{
/// @brief Once<T> models an optional value that is initialized at most once.
template<typename T>
class Once
{
public:

    /// @brief operator() returns an immutable reference to the value managed by this Once<T> instance.
    /// @throws boost::bad_optional_access if the managed value is empty.
    const T& operator()() const
    {
        return value.value();
    }
    /// @brief operator() returns an immutable reference to the value managed by this Once<T> instance,
    /// using f to initialize an instance if needed.
    /// @throws if f throws.
    template<typename F>
    const T& operator()(const F& f) const
    {
        std::call_once(init_once, [this, &f]()
        {
            value = f();
        });

        return *value;
    }

private:
    mutable std::once_flag init_once;
    mutable boost::optional<T> value;
};
}
}

#endif // BIOMETRY_UTIL_ONCE_H_
