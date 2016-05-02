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

#ifndef UTIL_SYNCHRONIZED_H_
#define UTIL_SYNCHRONIZED_H_

#include <mutex>
#include <type_traits>

namespace util
{
/// @brief Synchronized<T> bundles together a value and a mutex guarding it.
template<typename T>
class Synchronized
{
public:
    typedef T ValueType;

    /// @brief Synchronized creates a new instance, initializing the value to t.
    explicit Synchronized(const T& t = T{}) : value{t}
    {
    }

    /// @brief synchronized invokes the given functor f with the locked, mutable instance managed by this Synchronized<T> instance.
    template<typename F>
    void synchronized(const F& f)
    {
        std::lock_guard<std::mutex> lg{guard};
        f(value);
    }

private:
    std::mutex guard;
    T value;
};
}

#endif // UTIL_SYNCHRONIZED_H_
