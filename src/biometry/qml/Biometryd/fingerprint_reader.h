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
class FingerprintReaderHints : public QObject
{
    Q_OBJECT
    Q_ENUMS(Direction)
    Q_PROPERTY(bool hasMainClusterIdentified READ hasMainClusterIdentified WRITE setHasMainClusterIdentified)
    Q_PROPERTY(Direction suggestedDirectionOfNextTouch READ suggestedDirectionOfNextTouch WRITE setSuggestedDirectionOfNextTouch)
    Q_PROPERTY(QList<QRect> masks READ masks WRITE setMasks)

public:
    static constexpr const char* key{"fingerprintReaderHints"};

    enum class Direction
    {
        NotAvailable    = biometry::devices::FingerprintReader::Direction::not_available,
        SouthWest       = biometry::devices::FingerprintReader::Direction::sout_west,
        South           = biometry::devices::FingerprintReader::Direction::south,
        SouthEast       = biometry::devices::FingerprintReader::Direction::south_east,
        NorthWest       = biometry::devices::FingerprintReader::Direction::north_west,
        North           = biometry::devices::FingerprintReader::Direction::north,
        NorthEast       = biometry::devices::FingerprintReader::Direction::north_east,
        East            = biometry::devices::FingerprintReader::Direction::east,
        West            = biometry::devices::FingerprintReader::Direction::west
    };

    FingerprintReaderHints();
    FingerprintReaderHints(const FingerprintReaderHints& rhs);

    Q_INVOKABLE bool hasMainClusterIdentified() const;
    Q_INVOKABLE void setHasMainClusterIdentified(bool);

    Q_INVOKABLE Direction suggestedDirectionOfNextTouch() const;
    Q_INVOKABLE void setSuggestedDirectionOfNextTouch(Direction);

    Q_INVOKABLE QList<QRect> masks() const;
    Q_INVOKABLE void setMasks(QList<QRect>);

private:
    bool hasMainClusterIdentified_{false};
    Direction suggestedDirectionOfNextTouch_{Direction::NotAvailable};
    QList<QRect> masks_;
};
}
}

Q_DECLARE_METATYPE(biometry::qml::FingerprintReaderHints)

#endif // BIOMETRYD_QML_FINGERPRINT_READER_H_
