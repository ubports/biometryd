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

#include <biometry/qml/Biometryd/plugin.h>

#include <biometry/version.h>
#include <biometry/dbus/service.h>

#include <biometry/qml/Biometryd/device.h>
#include <biometry/qml/Biometryd/identifier.h>
#include <biometry/qml/Biometryd/operation.h>
#include <biometry/qml/Biometryd/service.h>
#include <biometry/qml/Biometryd/template_store.h>
#include <biometry/qml/Biometryd/user.h>

#include <QPoint>
#include <QDebug>
#include <QtQml>

template<typename... T>
std::runtime_error not_implemented(T&&...) { return std::runtime_error{"not implemented"}; }
namespace
{


struct NullTemplateStore : public biometry::TemplateStore
{
    biometry::Operation<SizeQuery>::Ptr size(const biometry::Application&, const biometry::User&) override
    {
        return biometry::Operation<SizeQuery>::Ptr{};
    }

    biometry::Operation<biometry::TemplateStore::List>::Ptr list(const biometry::Application&, const biometry::User&) override
    {
        return biometry::Operation<List>::Ptr{};
    }

    biometry::Operation<Enrollment>::Ptr enroll(const biometry::Application&, const biometry::User&) override
    {
        return biometry::Operation<Enrollment>::Ptr{};
    }

    biometry::Operation<Removal>::Ptr remove(const biometry::Application&, const biometry::User&, biometry::TemplateStore::TemplateId) override
    {
        return biometry::Operation<Removal>::Ptr{};
    }

    biometry::Operation<Clearance>::Ptr clear(const biometry::Application&, const biometry::User&) override
    {
        return biometry::Operation<Clearance>::Ptr{};
    }
};

struct NullIdentifier : public biometry::Identifier
{
    biometry::Operation<biometry::Identification>::Ptr identify_user(const biometry::Application&, const biometry::Reason&) override
    {
        return biometry::Operation<biometry::Identification>::Ptr{};
    }
};

struct NullDevice : public biometry::Device
{
    biometry::TemplateStore& template_store() override
    {
        return template_store_;
    }

    biometry::Identifier& identifier() override
    {
        return identifier_;
    }

    biometry::Verifier& verifier() override
    {
        throw not_implemented();
    }

    NullTemplateStore template_store_;
    NullIdentifier identifier_;
};

struct NullService : public biometry::Service
{
    std::shared_ptr<biometry::Device> default_device() const
    {
        return default_device_;
    }

    std::shared_ptr<NullDevice> default_device_{std::make_shared<NullDevice>()};
};
}

biometry::qml::Plugin::Plugin(QObject* parent) : QQmlExtensionPlugin{parent}
{
}

void biometry::qml::Plugin::registerTypes(const char *uri)
{
    Q_ASSERT(uri == std::string(biometry::qml::Plugin::ns));

    qmlRegisterType<biometry::qml::User>(uri, Plugin::major, Plugin::minor, "User");

    qmlRegisterUncreatableType<biometry::qml::Device>(uri, Plugin::major, Plugin::minor, "Device", "Rely on Biometryd.instance");
    qmlRegisterUncreatableType<biometry::qml::Identification>(uri, Plugin::major, Plugin::minor, "Identification", "Rely on Biometryd.instance");
    qmlRegisterUncreatableType<biometry::qml::Identifier>(uri, Plugin::major, Plugin::minor, "Identifier", "Rely on Biometryd.instance");

    qmlRegisterUncreatableType<biometry::qml::Operation>(uri, Plugin::major, Plugin::minor, "Operation", "Rely on Biometryd.instance");
    qmlRegisterUncreatableType<biometry::qml::SizeQuery>(uri, Plugin::major, Plugin::minor, "SizeQuery", "Rely on Biometryd.instance");
    qmlRegisterUncreatableType<biometry::qml::Enrollment>(uri, Plugin::major, Plugin::minor, "Enrollment", "Rely on Biometryd.instance");
    qmlRegisterUncreatableType<biometry::qml::Clearance>(uri, Plugin::major, Plugin::minor, "Clearance", "Rely on Biometryd.instance");
    qmlRegisterUncreatableType<biometry::qml::TemplateStore>(uri, Plugin::major, Plugin::minor, "TemplateStore", "Rely on Biometryd.instance");
    qmlRegisterSingletonType<biometry::qml::Service>(
                uri, Plugin::major, Plugin::minor, "Biometryd",
                [](QQmlEngine*, QJSEngine*) -> QObject*
                {
                    return qgetenv("BIOMETRYD_QML_ENABLE_TESTING") != QByteArray{"1"} ?
                        new biometry::qml::Service{biometry::dbus::Service::create_stub()} :
                        new biometry::qml::Service{std::make_shared<NullService>()};
                });
}
