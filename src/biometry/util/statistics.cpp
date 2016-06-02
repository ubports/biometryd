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

#include <biometry/util/statistics.h>

biometry::util::Statistics& biometry::util::Statistics::update(double observation)
{
    accumulator(observation); return *this;
}

double biometry::util::Statistics::min() const
{
    return boost::accumulators::min(accumulator);
}

double biometry::util::Statistics::mean() const
{
    return boost::accumulators::mean(accumulator);
}

double biometry::util::Statistics::variance() const
{
    return boost::accumulators::variance(accumulator);
}

double biometry::util::Statistics::max() const
{
    return boost::accumulators::max(accumulator);
}
