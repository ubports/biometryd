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

#include <biometry/devices/fingerprint_reader.h>

#include <gtest/gtest.h>

TEST(FingerprintReaderGuidanceHints, to_and_from_dictionary_works)
{
    biometry::devices::FingerprintReader::GuidedEnrollment::Hints g1;
    g1.is_main_cluster_identified = true;
    g1.suggested_next_direction = biometry::devices::FingerprintReader::Direction::east;
    g1.masks = std::vector<biometry::Rectangle>{biometry::Rectangle{{42, 42}, {43, 43}}};

    auto dict = g1.to_dictionary();

    biometry::devices::FingerprintReader::GuidedEnrollment::Hints g2;
    g2.from_dictionary(dict);

    EXPECT_EQ(g1, g2);
}
