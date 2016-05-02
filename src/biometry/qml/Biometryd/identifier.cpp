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

#include <biometry/qml/Biometryd/identifier.h>
#include <biometry/qml/Biometryd/user.h>

#include <biometry/application.h>
#include <biometry/reason.h>
#include <biometry/user.h>

/// @brief Identifier initializes a new instance with impl and parent.
biometry::qml::Identifier::Identifier(const std::reference_wrapper<biometry::Identifier>& impl, QObject* parent)
    : QObject{parent},
      impl{impl}
{
}

/// @brief size returns an operation querying the size of templates enrolled for user.
biometry::qml::Identification* biometry::qml::Identifier::identifyUser()
{
    return new Identification{impl.get().identify_user(biometry::Application::system(), biometry::Reason::unknown()), this};
}
