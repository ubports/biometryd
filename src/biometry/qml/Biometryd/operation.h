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

#ifndef BIOMETRYD_QML_OPERATION_H_
#define BIOMETRYD_QML_OPERATION_H_

#include <biometry/operation.h>
#include <biometry/reason.h>
#include <biometry/user.h>
#include <biometry/visibility.h>

#include <biometry/devices/fingerprint_reader.h>

#include <biometry/qml/Biometryd/converter.h>
#include <biometry/qml/Biometryd/fingerprint_reader.h>

#include <QCoreApplication>
#include <QDebug>
#include <QEvent>
#include <QPointer>
#include <QObject>
#include <QVariantMap>
#include <QRect>
#include <QVector>

#include <functional>
#include <memory>

namespace biometry
{
namespace qml
{
namespace traits
{
template<typename T>
struct Result
{
    static QVariant to_variant(const T&);
};

template<>
struct Result<biometry::Void>
{
    static QVariant to_variant(const biometry::Void&)
    {
        return QVariant{};
    }
};

template<>
struct Result<biometry::User>
{
    static QVariant to_variant(const biometry::User& user)
    {
        return QVariant{user.id};
    }
};

template<>
struct Result<biometry::TemplateStore::SizeQuery::Result>
{
    static QVariant to_variant(const biometry::TemplateStore::SizeQuery::Result& sq)
    {
        return QVariant{sq};
    }
};

template<>
struct Result<biometry::TemplateStore::TemplateId>
{
    static QVariant to_variant(const biometry::TemplateStore::TemplateId& id)
    {
        return QVariant{qulonglong{id}};
    }
};

template<>
struct Result<std::vector<biometry::TemplateStore::TemplateId>>
{
    static QVariant to_variant(const std::vector<biometry::TemplateStore::TemplateId>& ids)
    {
        QVariantList lids; for (const auto& id : ids) lids.push_back(Result<biometry::TemplateStore::TemplateId>::to_variant(id));
        return QVariant::fromValue(lids);
    }
};
}

/// @brief DispatcherWithContext dispatches tasks onto the QCoreApplication main loop.
template<typename Context>
class BIOMETRY_DLL_PUBLIC DispatcherWithContext : public QObject
{
private:
    /// @brief Task is the actual event used for transporting dispatched tasks
    /// from one thread to the other.
    class Task : public QEvent
    {
    public:
        /// @brief type returns the QEvent::Type assigned to DispatcherWithContext<T>::Task.
        static QEvent::Type type()
        {
            static const QEvent::Type t = static_cast<QEvent::Type>(QEvent::registerEventType());
            return t;
        }

        /// @brief Task initializes a new instance with the given null-ary void functor.
        Task(const std::function<void()>& f) : QEvent{Task::type()}, f{f}
        {
        }

        /// @brief run puts the contained functor to execution.
        void run()
        {
            f();
        }

    private:
        std::function<void()> f;
    };

public:
    /// @brief Dispatcher initializes a default instance.
    DispatcherWithContext() = default;

    /// @brief event processes events of type Task.
    bool event(QEvent* ev) override
    {
        if (ev->type() == Task::type())
        {
            reinterpret_cast<Task*>(ev)->run();
            return true;
        }

        return QObject::event(ev);
    }

    /// @brief finalize schedules deletion of this instance onto the QCoreApplication main loop.
    void finalize()
    {
        QCoreApplication::instance()->postEvent(this, new Task{[this]() { delete this; }});
    }

    /// @brief dispatch enqueues the given task for execution on the QCoreApplication
    /// main loop.
    void dispatch(const std::shared_ptr<Context>& ctxt, const std::function<void()>& task)
    {
        QCoreApplication::instance()->postEvent(this, new Task{[ctxt, task]() { task();}});
    }    
};

/// @brief Observer monitors an Operation.
class BIOMETRY_DLL_PUBLIC Observer : public QObject
{
    Q_OBJECT
public:
    /// @brief Observer initializes a new instance with the given parent.
    explicit Observer(QObject* parent = 0);

    /// @brief started is emitted when the state changes to started.
    Q_SIGNAL void started();
    /// @brief progressed is emitted when the overall operation progresses towards completion.
    /// @param percent Overall completion status of the operation, in [0,1], -1 indicates indeterminate.
    /// @param details Additional details about the operation.
    Q_SIGNAL void progressed(double percent, const QVariantMap& details);
    /// @brief canceled is emitted when the operation has been canceled.
    /// @param reason The human readable reason for cancelling the operation.
    Q_SIGNAL void canceled(const QString& reason);
    /// @brief failed is emitted when the operation fails to complete.
    /// @param reason The human readable reason for the failure.
    Q_SIGNAL void failed(const QString& reason);
    /// @brief succeeded is emitted when the operation completes successfully.
    /// @param result The result of the operation, might be empty.
    Q_SIGNAL void succeeded(const QVariant& result);
};

/// @brief Operation models an arbitrary operation as an observable state machine.
/// @ingroup qml
///
/// States are as follows:
///   -> Armed (the state right after construction)
///   start() -> Started
///           cancel() -> Cancelled
///           -> Failed
///           -> Succeeded
///
class BIOMETRY_DLL_PUBLIC Operation : public QObject
{
    Q_OBJECT
public:
    /// @brief Operation initializes a new instance with the given impl and parent.
    explicit Operation(QObject* parent);

    /// @brief start requests the operation to be started.
    /// @param observer Receiver for status updates about the operation.
    /// @return true if the issueing the request succeeded, false otherwise.
    Q_INVOKABLE virtual bool start(Observer* observer) = 0;
    /// @brief cancel requests the operation to be aborted.
    /// @return true if issueing the request succeeded, false otherwise.
    Q_INVOKABLE virtual bool cancel() = 0;
};

/// @brief TypedOperation implements Operation dispatching to a biometry::Operation<T>.
template<typename T>
class TypedOperation : public Operation
{
public:

    class Observer : public biometry::Operation<T>::Observer, public std::enable_shared_from_this<Observer>
    {
    public:
        typedef std::shared_ptr<Observer> Ptr;

        using typename biometry::Operation<T>::Observer::Progress;
        using typename biometry::Operation<T>::Observer::Reason;
        using typename biometry::Operation<T>::Observer::Error;
        using typename biometry::Operation<T>::Observer::Result;                

        static Ptr create(qml::Observer* observer)
        {
            return Ptr{new Observer{observer}};
        }

        ~Observer()
        {
            // observer is owned by the QML Engine. With that,
            // we only need to take care of our dispatcher instance.
            dispatcher->finalize();
        }

        void on_started() override
        {
            dispatcher->dispatch(Observer::shared_from_this(), [this]()
            {
                if (observer) QMetaObject::invokeMethod(observer, "started", Qt::AutoConnection);
            });
        }

        void on_progress(const Progress& progress) override
        {
            QVariantMap vm;

            biometry::devices::FingerprintReader::GuidedEnrollment::Hints hints;
            hints.from_dictionary(progress.details);

            if (hints.is_finger_present)
                vm[biometry::devices::FingerprintReader::GuidedEnrollment::Hints::key_is_finger_present] =
                        *hints.is_finger_present;

            if (hints.is_main_cluster_identified)
                vm[biometry::devices::FingerprintReader::GuidedEnrollment::Hints::key_is_main_cluster_identified] =
                        *hints.is_main_cluster_identified;

            if (hints.suggested_next_direction)
                vm[biometry::devices::FingerprintReader::GuidedEnrollment::Hints::key_suggested_next_direction].setValue(
                        Converter::convert(*hints.suggested_next_direction));

            if (hints.masks)
                vm[biometry::devices::FingerprintReader::GuidedEnrollment::Hints::key_masks] =
                        Converter::convert(*hints.masks);

            dispatcher->dispatch(Observer::shared_from_this(), [this, progress, vm]()
            {
                if (observer) QMetaObject::invokeMethod(observer, "progressed", Qt::AutoConnection,
                                                        Q_ARG(double, *progress.percent),
                                                        Q_ARG(QVariantMap, vm));
            });
        }

        void on_canceled(const Reason& reason) override
        {
            dispatcher->dispatch(Observer::shared_from_this(), [this, reason]()
            {
                if (observer) QMetaObject::invokeMethod(observer, "canceled", Qt::AutoConnection,
                                                        Q_ARG(QString, QString::fromStdString(reason)));
            });
        }

        void on_failed(const Error& error) override
        {
            dispatcher->dispatch(Observer::shared_from_this(), [this, error]()
            {
                if (observer) QMetaObject::invokeMethod(observer, "failed", Qt::AutoConnection,
                                                        Q_ARG(QString, QString::fromStdString(error)));
            });
        }

        void on_succeeded(const Result& result) override
        {
            dispatcher->dispatch(Observer::shared_from_this(), [this, result]()
            {
                if (observer) QMetaObject::invokeMethod(observer, "succeeded", Qt::AutoConnection,
                                                        Q_ARG(QVariant, traits::Result<Result>::to_variant(result)));
            });
        }

    private:
        Observer(qml::Observer* observer)
            : observer{observer},
              dispatcher{new DispatcherWithContext<Observer>{}}
        {
        }

        QPointer<qml::Observer> observer;
        DispatcherWithContext<Observer>* dispatcher;
    };

    /// @brief TypedOperation initializes a new instance with the given impl and parent.
    TypedOperation(const typename biometry::Operation<T>::Ptr& impl, QObject* parent)
        : Operation{parent},
          impl{impl}
    {
    }

    // From operation
    bool start(qml::Observer* observer) override
    {
        try
        {
            impl->start_with_observer(Observer::create(observer));
            return true;
        }
        catch(...)
        {
            return false;
        }
    }

    bool cancel() override
    {
        try
        {
            impl->cancel();
            return true;
        }
        catch (...)
        {
            return false;
        }
    }

private:
    /// @cond
    typename biometry::Operation<T>::Ptr impl;
    /// @endcond
};
}
}

#endif // BIOMETRYD_QML_OPERATION_H_
