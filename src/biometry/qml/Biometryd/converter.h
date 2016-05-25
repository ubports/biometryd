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

#ifndef BIOMETRYD_QML_CONVERTER_H_
#define BIOMETRYD_QML_CONVERTER_H_

#include <biometry/devices/fingerprint_reader.h>
#include <biometry/qml/Biometryd/fingerprint_reader.h>

#include <QList>
#include <QRectF>

namespace biometry
{
struct Rectangle;
namespace qml
{
/// @brief Converter maps biometry::* to Qt- and biometry::qml::* types.
struct Converter
{
    Converter() = delete;

    /// @brief convert returns a QRect with the same coordinates as rect.
    static QRectF convert(const biometry::Rectangle& rect);
    /// @brief convert returns a QList<QRect> containing the rectangles given in rects.
    static QVariantList convert(const std::vector<biometry::Rectangle>& rects);
    /// @brief convert returns the enum value correspondig to dir.
    static FingerprintReader::Direction convert(biometry::devices::FingerprintReader::Direction dir);
};
}
}

#endif // BIOMETRYD_QML_CONVERTER_H_
