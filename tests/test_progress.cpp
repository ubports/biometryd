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

#include <biometry/progress.h>

#include <gtest/gtest.h>

#include <sstream>

TEST(Progress, inserts_correctly_into_output_stream)
{
    biometry::Progress progress{biometry::Percent::from_raw_value(0.5), biometry::Dictionary{}};
    std::stringstream ss; ss << progress;
    EXPECT_EQ("Progress: 50%", ss.str());
}
