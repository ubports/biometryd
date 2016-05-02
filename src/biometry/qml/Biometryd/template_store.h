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

#ifndef BIOMETRYD_QML_TEMPLATE_STORE_H_
#define BIOMETRYD_QML_TEMPLATE_STORE_H_

#include <biometry/template_store.h>
#include <biometry/visibility.h>

#include <biometry/qml/Biometryd/operation.h>
#include <biometry/qml/Biometryd/user.h>

#include <QObject>
#include <QString>

namespace biometry
{

namespace qml
{
class Operation;
class User;

/// @cond
class BIOMETRY_DLL_PUBLIC SizeQuery : public TypedOperation<biometry::TemplateStore::SizeQuery>
{
    Q_OBJECT
public:
    SizeQuery(const biometry::Operation<biometry::TemplateStore::SizeQuery>::Ptr& impl, QObject* parent)
        : TypedOperation<biometry::TemplateStore::SizeQuery>{impl, parent}
    {
    }
};

class BIOMETRY_DLL_PUBLIC Enrollment : public TypedOperation<biometry::TemplateStore::Enrollment>
{
    Q_OBJECT
public:
    Enrollment(const biometry::Operation<biometry::TemplateStore::Enrollment>::Ptr& impl, QObject* parent)
        : TypedOperation<biometry::TemplateStore::Enrollment>{impl, parent}
    {
    }
};

class BIOMETRY_DLL_PUBLIC Clearance : public TypedOperation<biometry::TemplateStore::Clearance>
{
    Q_OBJECT
public:
    Clearance(const biometry::Operation<biometry::TemplateStore::Clearance>::Ptr& impl, QObject* parent)
        : TypedOperation<biometry::TemplateStore::Clearance>{impl, parent}
    {
    }
};
/// @endcond

/// @brief TemplateStore models a collection of templates.
/// @ingroup qml
class BIOMETRY_DLL_PUBLIC TemplateStore : public QObject
{
    Q_OBJECT
public:
    /// @brief TemplateStore initializes a new instance with impl and parent.
    TemplateStore(const std::reference_wrapper<biometry::TemplateStore>& impl, QObject* parent);

    /// @brief size returns an operation querying the size of templates enrolled for user.
    Q_INVOKABLE biometry::qml::SizeQuery* size(biometry::qml::User* user);
    /// @brief enroll returns an operation enrolling a new template for user.
    Q_INVOKABLE biometry::qml::Enrollment* enroll(biometry::qml::User* user);
    /// @brief clear returns an operation removing all templates for app and user.
    Q_INVOKABLE biometry::qml::Clearance* clear(biometry::qml::User* user);

private:
    /// @cond
    std::reference_wrapper<biometry::TemplateStore> impl;
    /// @endcond
};
}
}

#endif // BIOMETRYD_QML_TEMPLATE_STORE_H_
