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

#ifndef BIOMETRY_UTIL_STATISTICS_H_
#define BIOMETRY_UTIL_STATISTICS_H_

#include <biometry/visibility.h>

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/max.hpp>
#include <boost/accumulators/statistics/min.hpp>
#include <boost/accumulators/statistics/variance.hpp>

namespace biometry
{
namespace util
{
/// @brief Statistics helps in tracking min/max/mean/variance of a sample.
class BIOMETRY_DLL_PUBLIC Statistics
{
public:
    /// @brief update adds the observation to the statistics.
    Statistics& update(double observation);

    /// @brief min returns the current min of the sample seen thus far.
    double min() const;
    /// @brief mean returns the current mean of the sample seen thus far.
    double mean() const;
    /// @brief variance returns the current variance of the sample seen thus far.
    double variance() const;
    /// @brief max returns the current max of the sample seen thus far.
    double max() const;

private:
    /// @cond
    typedef boost::accumulators::accumulator_set
    <
        double,
        boost::accumulators::stats
        <
            boost::accumulators::tag::min,
            boost::accumulators::tag::max,
            boost::accumulators::tag::mean,
            boost::accumulators::tag::variance
        >
    > Accumulator;

    Accumulator accumulator;
    /// @endcond
};
}
}

#endif // BIOMETRY_UTIL_STATISTICS_H_
