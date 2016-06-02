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

#include <biometry/util/benchmark.h>

#include <chrono>

namespace
{
template<typename Duration>
std::chrono::microseconds to_ms(const Duration& duration)
{
    return std::chrono::duration_cast<std::chrono::microseconds>(duration);
}
}

biometry::util::Benchmark::Benchmark(const std::function<void()>& operation) : operation_{operation}, trials_{1}
{
}

biometry::util::Benchmark& biometry::util::Benchmark::trials(std::size_t value)
{
    trials_ = value;
    return *this;
}

biometry::util::Benchmark biometry::util::Benchmark::trials(std::size_t value) const
{
    auto copy = *this;
    return copy.trials(value);
}

biometry::util::Benchmark& biometry::util::Benchmark::on_progress(const std::function<void(std::size_t, std::size_t)>& f)
{
    on_progress_ = f;
    return *this;
}

biometry::util::Benchmark biometry::util::Benchmark::on_progress(const std::function<void(std::size_t, std::size_t)>& f) const
{
    auto copy = *this;
    return copy.on_progress(f);
}

biometry::util::Benchmark& biometry::util::Benchmark::on_error(const std::function<bool()>& f)
{
    on_error_ = f;
    return *this;
}

biometry::util::Benchmark biometry::util::Benchmark::on_error(const std::function<bool()>& f) const
{
    auto copy = *this;
    return copy.on_error(f);
}

biometry::util::Statistics biometry::util::Benchmark::run() const
{
    biometry::util::Statistics stats;
    for (std::size_t i = 1; i <= trials_; i++)
    {
        if (on_progress_) on_progress_(i, trials_);

        auto before = std::chrono::high_resolution_clock::now();
        {
            try
            {
                operation_();
            }
            catch(...)
            {
                if (on_error_ && on_error_())
                    continue;

                std::rethrow_exception(std::current_exception());
            }
        }
        stats.update(to_ms(std::chrono::high_resolution_clock::now() - before).count());
    }

    return stats;
}
