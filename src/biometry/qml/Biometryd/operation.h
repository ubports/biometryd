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

#include <QDebug>
#include <QObject>
#include <QVariantMap>
#include <QRect>
#include <QVector>

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

    class Observer : public biometry::Operation<T>::Observer
    {
    public:
        using typename biometry::Operation<T>::Observer::Progress;
        using typename biometry::Operation<T>::Observer::Reason;
        using typename biometry::Operation<T>::Observer::Error;
        using typename biometry::Operation<T>::Observer::Result;

        Observer(qml::Observer* observer) : observer{observer}
        {
        }

        void on_started() override
        {
            QMetaObject::invokeMethod(observer, "started", Qt::QueuedConnection);
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

            QMetaObject::invokeMethod(observer, "progressed", Qt::QueuedConnection,
                                      Q_ARG(double, *progress.percent),
                                      Q_ARG(QVariantMap, vm));
        }

        void on_canceled(const Reason& reason) override
        {
            QMetaObject::invokeMethod(observer, "canceled", Qt::QueuedConnection,
                                      Q_ARG(QString, QString::fromStdString(reason)));
        }

        void on_failed(const Error& error) override
        {
            QMetaObject::invokeMethod(observer, "failed", Qt::QueuedConnection,
                                      Q_ARG(QString, QString::fromStdString(error)));
        }

        void on_succeeded(const Result& result) override
        {
            QMetaObject::invokeMethod(observer, "succeeded", Qt::QueuedConnection,
                                      Q_ARG(QVariant, traits::Result<Result>::to_variant(result)));
        }

    private:
        qml::Observer* observer;
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
            impl->start_with_observer(std::make_shared<Observer>(observer));
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
