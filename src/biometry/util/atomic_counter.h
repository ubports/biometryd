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

#ifndef BIOMETRY_UTIL_ATOMIC_COUNTER_H_
#define BIOMETRY_UTIL_ATOMIC_COUNTER_H_

#include <biometry/visibility.h>

#include <cstdint>

#include <atomic>

namespace biometry
{
namespace util
{
/// @brief AtomicCounter models an atomic counter.
class BIOMETRY_DLL_PUBLIC AtomicCounter
{
public:
    /// @brief AtomicCounter initializes a new instance to the given value.
    explicit AtomicCounter(std::uint64_t value = 0);

    /// @brief increment increments the value of the counter and returns the previously stored value.
    std::uint64_t increment();

private:
    /// @cond
    std::atomic<std::uint64_t> counter;
    /// @endcond
};

template<typename Tag>
inline AtomicCounter& counter()
{
    static AtomicCounter instance;
    return instance;
}
}
}

#endif // BIOMETRY_UTIL_ATOMIC_COUNTER_H_
