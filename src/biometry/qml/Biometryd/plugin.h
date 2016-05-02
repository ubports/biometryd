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

#ifndef BIOMETRYD_QML_PLUGIN_H_
#define BIOMETRYD_QML_PLUGIN_H_

#include <biometry/version.h>
#include <biometry/visibility.h>

#include <QQmlExtensionPlugin>

namespace biometry
{
namespace qml
{
/// @defgroup qml QML bindings for biometryd

/// @cond
class BIOMETRY_DLL_PUBLIC Plugin : public QQmlExtensionPlugin
{
    static constexpr const char* ns{"Biometryd"};
    static constexpr std::uint32_t major = 0;
    static constexpr std::uint32_t minor = 0;

    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")
public:
    explicit Plugin(QObject* parent = nullptr);
    // From QQmlExtensionPlugin
    void registerTypes(const char *uri) override;
};
/// @endcond
}
}

#endif // BIOMETRYD_QML_PLUGIN_H_
