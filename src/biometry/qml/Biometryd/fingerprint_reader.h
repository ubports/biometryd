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

#ifndef BIOMETRYD_QML_FINGERPRINT_READER_H_
#define BIOMETRYD_QML_FINGERPRINT_READER_H_

#include <biometry/devices/fingerprint_reader.h>

#include <QList>
#include <QObject>
#include <QRect>

namespace biometry
{
namespace qml
{
/// @brief A FingerprintReader bundles well-known names of hints passed to Observers.
class FingerprintReader : public QObject
{
    Q_OBJECT
    Q_ENUMS(Direction)
    Q_PROPERTY(QString isFingerPresent READ isFingerPresent)
    Q_PROPERTY(QString hasMainClusterIdentified READ hasMainClusterIdentified)
    Q_PROPERTY(QString suggestedNextDirection READ suggestedNextDirection)
    Q_PROPERTY(QString masks READ masks)

public:
    /// @brief Direction enumerates all known direction hints.
    enum class Direction
    {
        NotAvailable    = int(biometry::devices::FingerprintReader::Direction::not_available),
	SouthWest       = int(biometry::devices::FingerprintReader::Direction::south_west),
	South           = int(biometry::devices::FingerprintReader::Direction::south),
        SouthEast       = int(biometry::devices::FingerprintReader::Direction::south_east),
        NorthWest       = int(biometry::devices::FingerprintReader::Direction::north_west),
        North           = int(biometry::devices::FingerprintReader::Direction::north),
        NorthEast       = int(biometry::devices::FingerprintReader::Direction::north_east),
        East            = int(biometry::devices::FingerprintReader::Direction::east),
	    West            = int(biometry::devices::FingerprintReader::Direction::west)
    };

    /// @brief FingerprintReader initializes a new instance with the given parent.
    FingerprintReader(QObject* parent = nullptr);

    /// @brief isFingerPresent returns the key of the isFingerPresent hint.
    Q_INVOKABLE QString isFingerPresent() const;
    /// @brief hasMainClusterIdentified returns the key of the hasMainClusterIdentified hint.
    Q_INVOKABLE QString hasMainClusterIdentified() const;
    /// @brief suggestedNextDirection returns the key of the suggestedNextDirection hint.
    Q_INVOKABLE QString suggestedNextDirection() const;
    /// @brief estimatedFingerSize returns the key of the estimatedFingerSize hint.
    Q_INVOKABLE QString estimatedFingerSize() const;
    /// @brief masks returns the key of the masks hint.
    Q_INVOKABLE QString masks() const;
};
}
}

Q_DECLARE_METATYPE(biometry::qml::FingerprintReader::Direction)

#endif // BIOMETRYD_QML_FINGERPRINT_READER_H_
