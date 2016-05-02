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

#include <biometry/qml/Biometryd/converter.h>

#include <biometry/geometry.h>

QRect biometry::qml::Converter::convert(const biometry::Rectangle& rect)
{
    return QRect
    {
        static_cast<int>(rect.top_left.x),
        static_cast<int>(rect.top_left.y),
        static_cast<int>(rect.bottom_right.x),
        static_cast<int>(rect.bottom_right.y)
    };
}


