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

#ifndef BIOMETRYD_QML_DEVICE_H_
#define BIOMETRYD_QML_DEVICE_H_

#include <biometry/device.h>
#include <biometry/visibility.h>

#include <QObject>

#include <memory>

namespace biometry
{
namespace qml
{
/// @cond
class Identifier;
class TemplateStore;
/// @endcond

/// @brief Device models a biometric device.
/// @ingroup qml
class BIOMETRY_DLL_PUBLIC Device : public QObject
{
    Q_OBJECT
    Q_PROPERTY(biometry::qml::TemplateStore* templateStore READ templateStore CONSTANT)
    Q_PROPERTY(biometry::qml::Identifier* identifier READ identifier CONSTANT)
public:
    /// @brief Device initializes a new instance with device and parent.
    Device(const std::shared_ptr<biometry::Device>& impl, QObject* parent);

    /// @brief templateStore returns a TemplateStore instance.
    biometry::qml::TemplateStore* templateStore();
    /// @brief identifier returns an Identifier instance.
    biometry::qml::Identifier* identifier();

private:
    /// @cond
    std::shared_ptr<biometry::Device> impl;
    /// @endcond
};
}
}

#endif // BIOMETRYD_QML_DEVICE_H_
