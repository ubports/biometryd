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

#ifndef BIOMETRYD_QML_IDENTIFIER_H_
#define BIOMETRYD_QML_IDENTIFIER_H_

#include <biometry/identifier.h>
#include <biometry/visibility.h>

#include <biometry/qml/Biometryd/operation.h>

#include <QObject>
#include <QString>

namespace biometry
{
namespace qml
{
class User;

/// @cond
class BIOMETRY_DLL_PUBLIC Identification : public TypedOperation<biometry::Identification>
{
    Q_OBJECT
public:
    Identification(const biometry::Operation<biometry::Identification>::Ptr& impl, QObject* parent)
        : TypedOperation<biometry::Identification>{impl, parent}
    {
    }
};
/// @endcond

/// @brief TemplateStore models a collection of templates.
/// @ingroup qml
class BIOMETRY_DLL_PUBLIC Identifier : public QObject
{
    Q_OBJECT
public:
    /// @brief Identifier initializes a new instance with impl and parent.
    Identifier(const std::reference_wrapper<biometry::Identifier>& impl, QObject* parent);

    /// @brief size returns an operation querying the size of templates enrolled for user.
    Q_INVOKABLE biometry::qml::Identification* identifyUser();

private:
    /// @cond
    std::reference_wrapper<biometry::Identifier> impl;
    /// @endcond
};
}
}

#endif // BIOMETRYD_QML_IDENTIFIER_H_
