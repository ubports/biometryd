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

#ifndef BIOMETRY_UTIL_BENCHMARK_H_
#define BIOMETRY_UTIL_BENCHMARK_H_

#include <biometry/util/statistics.h>

#include <cstdint>

#include <functional>

namespace biometry
{
namespace util
{
/// @brief Benchmark executes a given operation for a number of trials and returns statistics about
/// the runtime of an individual iteration of the operation.
class Benchmark
{
public:
    /// @brief Benchmark initializes an instance with operation.
    explicit Benchmark(const std::function<void()>& operation);

    /// @brief trials adjusts the number of trials to value.
    Benchmark trials(std::size_t value) const;
    /// @brief trials adjusts the number of trials to value.
    Benchmark& trials(std::size_t value);
    /// @brief on_progress installs the given handler function.
    Benchmark on_progress(const std::function<void(std::size_t, std::size_t)>&) const;
    /// @brief on_progress installs the given handler function.
    Benchmark& on_progress(const std::function<void(std::size_t, std::size_t)>&);
    /// @brief on_error installs the given handler function.
    Benchmark on_error(const std::function<bool()>&) const;
    /// @brief on_error installs the given handler function.
    Benchmark& on_error(const std::function<bool()>&);

    /// @brief run executes the benchmark, accumulating statistics over the runtime.
    biometry::util::Statistics run() const;

private:
    /// @cond
    std::function<void()> operation_;
    std::size_t trials_;
    std::function<void(std::size_t, std::size_t)> on_progress_;
    std::function<bool()> on_error_;
    /// @endcond
};
}
}

#endif // BIOMETRY_UTIL_BENCHMARK_H_
