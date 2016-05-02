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

#include <biometry/user.h>
#include <biometry/version.h>
#include <biometry/devices/fingerprint_reader.h>

#include <biometry/dbus/service.h>

#include <biometry/qml/Biometryd/device.h>
#include <biometry/qml/Biometryd/fingerprint_reader.h>
#include <biometry/qml/Biometryd/identifier.h>
#include <biometry/qml/Biometryd/operation.h>
#include <biometry/qml/Biometryd/service.h>
#include <biometry/qml/Biometryd/template_store.h>
#include <biometry/qml/Biometryd/user.h>

#include <QPoint>
#include <QDebug>
#include <QtQml>

#include <boost/asio.hpp>

#include <functional>
#include <thread>

namespace
{
template<typename... T>
std::runtime_error not_implemented(T&&...) { return std::runtime_error{"not implemented"}; }

namespace for_testing
{
// We enable testing of projects using the QML bindings by providing an environment
// variable BIOMETRYD_QML_ENABLE_TESTING. If the variable is set to "1", we install a testing
// stack emulating the multi-threaded/async behavior of the production system within the test
// process.
class Dispatcher
{
public:
    static Dispatcher& instance()
    {
        static Dispatcher dispatcher;
        return dispatcher;
    }

    ~Dispatcher()
    {
        service.stop();
        if (t.joinable()) t.join();
    }

    void dispatch(const std::function<void()>& f)
    {
        service.post(f);
    }

private:
    Dispatcher()
        : keep_alive{service},
          t{[this]() { service.run(); }}
    {
    }

    boost::asio::io_service service;
    boost::asio::io_service::work keep_alive;
    std::thread t;

};

struct SizeQuery : public biometry::Operation<biometry::TemplateStore::SizeQuery>,
                       public std::enable_shared_from_this<for_testing::SizeQuery>
{
    void start_with_observer(const typename Observer::Ptr& observer) override
    {
        auto thiz = shared_from_this();
        Dispatcher::instance().dispatch([observer, thiz]()
        {
            observer->on_started();

            for (std::size_t i = 1; i <= 100; i++)
            {
                if (thiz->canceled)
                {
                    observer->on_canceled("Canceled due to request from client");
                    return;
                }

                observer->on_progress(biometry::Progress{biometry::Percent::from_raw_value(i/100.f), biometry::Dictionary{}});
                std::this_thread::sleep_for(std::chrono::milliseconds{15});
            }

            observer->on_succeeded(42);
        });
    }

    void cancel() override
    {
        canceled = true;
    }

    bool canceled{false};
};

struct Enrollment : public biometry::Operation<biometry::TemplateStore::Enrollment>,
                        public std::enable_shared_from_this<for_testing::Enrollment>
{
    void start_with_observer(const typename Observer::Ptr& observer) override
    {
        auto thiz = shared_from_this();
        Dispatcher::instance().dispatch([observer, thiz]()
        {
            observer->on_started();

            for (std::size_t i = 1; i <= 100; i++)
            {
                if (thiz->canceled)
                {
                    observer->on_canceled("Canceled due to request from client");
                    return;
                }

                biometry::devices::FingerprintReader::GuidedEnrollment::Hints hints;
                hints.is_main_cluster_identified = i > 85;
                hints.suggested_next_direction = static_cast<biometry::devices::FingerprintReader::Direction>(i % 9);
                hints.masks = { biometry::Rectangle{biometry::Point{10, 10}, biometry::Point{50, 50}},
                                biometry::Rectangle{biometry::Point{50, 50}, biometry::Point{90, 90}} };

                observer->on_progress(biometry::Progress{biometry::Percent::from_raw_value(i/100.f), hints.to_dictionary()});
                std::this_thread::sleep_for(std::chrono::milliseconds{15});
            }

            observer->on_succeeded(biometry::Void{});
        });
    }

    void cancel() override
    {
        canceled = true;
    }

    bool canceled{false};
};

struct Identification : public biometry::Operation<biometry::Identification>,
                           public std::enable_shared_from_this<for_testing::Identification>
{
    void start_with_observer(const typename Observer::Ptr& observer) override
    {
        auto thiz = shared_from_this();
        Dispatcher::instance().dispatch([observer, thiz]()
        {
            observer->on_started();

            for (std::size_t i = 1; i <= 100; i++)
            {
                if (thiz->canceled)
                {
                    observer->on_canceled("Canceled due to request from client");
                    return;
                }

                observer->on_progress(biometry::Progress{biometry::Percent::from_raw_value(i/100.f), biometry::Dictionary{}});
                std::this_thread::sleep_for(std::chrono::milliseconds{15});
            }

            observer->on_succeeded(biometry::User{42});
        });
    }

    void cancel() override
    {
        canceled = true;
    }

    bool canceled{false};
};

struct Clearance : public biometry::Operation<biometry::TemplateStore::Clearance>,
                       public std::enable_shared_from_this<for_testing::Clearance>
{
    void start_with_observer(const typename Observer::Ptr& observer) override
    {
        auto thiz = shared_from_this();
        Dispatcher::instance().dispatch([observer, thiz]()
        {
            observer->on_started();

            for (std::size_t i = 1; i <= 100; i++)
            {
                if (thiz->canceled)
                {
                    observer->on_canceled("Canceled due to request from client");
                    return;
                }

                observer->on_progress(biometry::Progress{biometry::Percent::from_raw_value(i/100.f), biometry::Dictionary{}});
                std::this_thread::sleep_for(std::chrono::milliseconds{15});
            }

            observer->on_succeeded(biometry::Void{});
        });
    }

    void cancel() override
    {
        canceled = true;
    }

    bool canceled{false};
};

struct TemplateStore : public biometry::TemplateStore
{
    biometry::Operation<SizeQuery>::Ptr size(const biometry::Application&, const biometry::User&) override
    {
        return std::make_shared<for_testing::SizeQuery>();
    }

    biometry::Operation<Enrollment>::Ptr enroll(const biometry::Application&, const biometry::User&) override
    {
        return std::make_shared<for_testing::Enrollment>();
    }

    biometry::Operation<Clearance>::Ptr clear(const biometry::Application&, const biometry::User&) override
    {
        return std::make_shared<for_testing::Clearance>();
    }
};

struct Identifier : public biometry::Identifier
{
    biometry::Operation<biometry::Identification>::Ptr identify_user(const biometry::Application&, const biometry::Reason&) override
    {
        return std::make_shared<for_testing::Identification>();
    }
};

struct Device : public biometry::Device
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

    TemplateStore template_store_;
    Identifier identifier_;
};

struct Service : public biometry::Service
{
    std::shared_ptr<biometry::Device> default_device() const
    {
        return default_device_;
    }

    std::shared_ptr<Device> default_device_{std::make_shared<for_testing::Device>()};
};
}
}

biometry::qml::Plugin::Plugin(QObject* parent) : QQmlExtensionPlugin{parent}
{
}

void biometry::qml::Plugin::registerTypes(const char *uri)
{
    Q_ASSERT(uri == std::string(biometry::qml::Plugin::ns));

    qmlRegisterType<biometry::qml::Observer>(uri, Plugin::major, Plugin::minor, "Observer");
    qmlRegisterType<biometry::qml::User>(uri, Plugin::major, Plugin::minor, "User");

    qmlRegisterUncreatableType<biometry::qml::Device>(uri, Plugin::major, Plugin::minor, "Device", "Rely on Biometryd.instance");
    qmlRegisterUncreatableType<biometry::qml::Identification>(uri, Plugin::major, Plugin::minor, "Identification", "Rely on Biometryd.instance");
    qmlRegisterUncreatableType<biometry::qml::Identifier>(uri, Plugin::major, Plugin::minor, "Identifier", "Rely on Biometryd.instance");

    qmlRegisterUncreatableType<biometry::qml::FingerprintReaderHints>(uri, Plugin::major, Plugin::minor, "FingerprintReaderHints", "Rely on Biometryd.instance");
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
                        new biometry::qml::Service{std::make_shared<for_testing::Service>()};
                });
}
