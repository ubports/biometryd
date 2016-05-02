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

#include <QRect>

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
    static QRect convert(const biometry::Rectangle& rect);

};
}
}

#endif // BIOMETRYD_QML_CONVERTER_H_
