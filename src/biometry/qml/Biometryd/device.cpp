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

#include <biometry/qml/Biometryd/device.h>

#include <biometry/qml/Biometryd/identifier.h>
#include <biometry/qml/Biometryd/template_store.h>

biometry::qml::Device::Device(const std::shared_ptr<biometry::Device>& impl, QObject* parent)
    : QObject{parent},
      impl{impl}
{
}

biometry::qml::TemplateStore* biometry::qml::Device::templateStore()
{
    return new TemplateStore{impl->template_store(), this};
}

biometry::qml::Identifier* biometry::qml::Device::identifier()
{
    return new Identifier{impl->identifier(), this};
}
