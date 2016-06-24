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

#include <biometry/qml/Biometryd/template_store.h>
#include <biometry/qml/Biometryd/user.h>

#include <biometry/application.h>
#include <biometry/user.h>


biometry::qml::TemplateStore::TemplateStore(const std::reference_wrapper<biometry::TemplateStore>& impl, QObject* parent)
    : QObject{parent},
      impl{impl}
{
}

biometry::qml::SizeQuery* biometry::qml::TemplateStore::size(User* user)
{
    return new biometry::qml::SizeQuery{impl.get().size(biometry::Application::system(), biometry::User{user->uid()}), this};
}

biometry::qml::List* biometry::qml::TemplateStore::list(User* user)
{
    return new biometry::qml::List{impl.get().list(biometry::Application::system(), biometry::User{user->uid()}), this};
}

biometry::qml::Enrollment* biometry::qml::TemplateStore::enroll(User* user)
{
    return new biometry::qml::Enrollment{impl.get().enroll(biometry::Application::system(), biometry::User{user->uid()}), this};
}

biometry::qml::Removal* biometry::qml::TemplateStore::remove(User* user, qulonglong id)
{
    return new biometry::qml::Removal{impl.get().remove(biometry::Application::system(), biometry::User{user->uid()}, id), this};
}

biometry::qml::Clearance* biometry::qml::TemplateStore::clear(User* user)
{
    return new biometry::qml::Clearance{impl.get().clear(biometry::Application::system(), biometry::User{user->uid()}), this};
}
