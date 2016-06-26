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

biometry::qml::FingerprintReader::FingerprintReader(QObject* parent)
    : QObject{parent}
{
}

QString biometry::qml::FingerprintReader::isFingerPresent() const
{
    static const QString s
    {
        biometry::devices::FingerprintReader::GuidedEnrollment::Hints::key_is_finger_present
    };
    return s;
}

QString biometry::qml::FingerprintReader::hasMainClusterIdentified() const
{
    static const QString s
    {
        biometry::devices::FingerprintReader::GuidedEnrollment::Hints::key_is_main_cluster_identified
    };
    return s;
}

QString biometry::qml::FingerprintReader::suggestedNextDirection() const
{
    static const QString s
    {
        biometry::devices::FingerprintReader::GuidedEnrollment::Hints::key_suggested_next_direction
    };
    return s;
}

QString biometry::qml::FingerprintReader::estimatedFingerSize() const
{
    static const QString s
    {
        biometry::devices::FingerprintReader::GuidedEnrollment::Hints::key_estimated_finger_size
    };
    return s;
}

QString biometry::qml::FingerprintReader::masks() const
{
    static const QString s
    {
        biometry::devices::FingerprintReader::GuidedEnrollment::Hints::key_masks
    };
    return s;
}
