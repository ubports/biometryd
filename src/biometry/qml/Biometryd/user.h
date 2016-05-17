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

#ifndef BIOMETRYD_QML_USER_H_
#define BIOMETRYD_QML_USER_H_

#include <biometry/visibility.h>

#include <QObject>

namespace biometry
{
namespace qml
{
/// @brief User models a user known to the system.
/// @ingroup qml
class BIOMETRY_DLL_PUBLIC User : public QObject
{
    Q_OBJECT
    Q_PROPERTY(unsigned int uid READ uid WRITE setUid NOTIFY uidChanged)
public:
    User(QObject* parent = nullptr);

    /// @brief id returns the numeric user id.
    Q_INVOKABLE unsigned int uid() const;

    /// @brief setId adjusts the numeric user id.
    Q_INVOKABLE void setUid(unsigned int id);

    /// @brief uidChanged is emitted with the newly set id.
    Q_SIGNAL void uidChanged(unsigned int id);
private:
    /// @cond
    unsigned int id_{0};
    /// @endcond
};

class BIOMETRY_DLL_PUBLIC Users : public QObject
{
    Q_OBJECT
public:
    Users(QObject* parent = nullptr);
};
}
}

#endif // BIOMETRYD_QML_USER_H_
