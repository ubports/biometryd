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

#include <biometry/qml/Biometryd/fingerprint_reader.h>

biometry::qml::FingerprintReaderHints::FingerprintReaderHints()
{
}

biometry::qml::FingerprintReaderHints::FingerprintReaderHints(const FingerprintReaderHints& rhs)
    : QObject{},
      hasMainClusterIdentified_{rhs.hasMainClusterIdentified_},
      suggestedDirectionOfNextTouch_{rhs.suggestedDirectionOfNextTouch_},
      masks_{rhs.masks_}
{
}

bool biometry::qml::FingerprintReaderHints::hasMainClusterIdentified() const
{
    return hasMainClusterIdentified_;
}

void biometry::qml::FingerprintReaderHints::setHasMainClusterIdentified(bool value)
{
    hasMainClusterIdentified_ = value;
}

biometry::qml::FingerprintReaderHints::Direction biometry::qml::FingerprintReaderHints::suggestedDirectionOfNextTouch() const
{
    return suggestedDirectionOfNextTouch_;
}

void biometry::qml::FingerprintReaderHints::setSuggestedDirectionOfNextTouch(Direction d)
{
    suggestedDirectionOfNextTouch_ = d;
}

QList<QRect> biometry::qml::FingerprintReaderHints::masks() const
{
    return masks_;
}

void biometry::qml::FingerprintReaderHints::setMasks(QList<QRect> masks)
{
    masks_ = masks;
}
