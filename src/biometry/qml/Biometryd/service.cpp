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

#include <biometry/qml/Biometryd/service.h>
#include <biometry/qml/Biometryd/device.h>

#include <biometry/dbus/service.h>

#include <QCoreApplication>

biometry::qml::Device* biometry::qml::Service::defaultDevice() const
{
    return defaultDevice_;
}

bool biometry::qml::Service::isAvailable() const
{
    return available_;
}

void biometry::qml::Service::setAvailable(bool available)
{
    if (available_ == available)
        return;

    Q_EMIT(availableChanged(available_ = available));
}

biometry::qml::Service::Service(const std::shared_ptr<biometry::Service>& impl, QObject* parent)
    : QObject{parent},
      available_{true},
      impl_{impl},
      defaultDevice_{new Device{impl->default_device(), this}}
{
}
