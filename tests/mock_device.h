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

#ifndef TESTING_MOCK_DEVICE_H_
#define TESTING_MOCK_DEVICE_H_

#include <biometry/application.h>
#include <biometry/reason.h>
#include <biometry/user.h>

#include <biometry/identifier.h>
#include <biometry/template_store.h>
#include <biometry/verifier.h>

#include <biometry/device.h>

#include <gmock/gmock.h>

namespace testing
{
template<typename T>
struct MockOperation : public biometry::Operation<T>
{
    // Safe us some typing
    typedef biometry::Operation<T> Super;

    using typename Super::Observer;
    using typename Super::Progress;
    using typename Super::Reason;
    using typename Super::Error;
    using typename Super::Result;


    // From biometry::Operation<T>
    MOCK_METHOD1_T(start_with_observer, void(const typename Observer::Ptr&));
    MOCK_METHOD0(cancel, void());

};

template<typename T>
struct MockObserver : public biometry::Operation<T>::Observer
{
    // Safe us some typing
    typedef typename biometry::Operation<T>::Observer Super;

    using typename Super::Progress;
    using typename Super::Reason;
    using typename Super::Error;
    using typename Super::Result;

    // From Operation<T>::Observer
    MOCK_METHOD0(on_started, void());
    MOCK_METHOD1_T(on_progress, void(const Progress&));
    MOCK_METHOD1_T(on_canceled, void(const Reason&));
    MOCK_METHOD1_T(on_failed, void(const Error&));
    MOCK_METHOD1_T(on_succeeded, void(const Result&));
};

struct MockTemplateStore : public biometry::TemplateStore
{
    MOCK_METHOD2(size, biometry::Operation<SizeQuery>::Ptr(const biometry::Application&, const biometry::User&));
    MOCK_METHOD2(enroll, biometry::Operation<Enrollment>::Ptr (const biometry::Application&, const biometry::User&));
    MOCK_METHOD2(clear, biometry::Operation<Clearance>::Ptr(const biometry::Application&, const biometry::User&));
};

struct MockIdentifier : public biometry::Identifier
{
    MOCK_METHOD2(identify_user, biometry::Operation<biometry::Identification>::Ptr(const biometry::Application&, const biometry::Reason&));
};

struct MockVerifier : public biometry::Verifier
{
    MOCK_METHOD3(verify_user, biometry::Operation<biometry::Verification>::Ptr(const biometry::Application&, const biometry::User&, const biometry::Reason&));
};

struct MockDevice : public biometry::Device
{
    // From biometry::Device
    MOCK_METHOD0(template_store,    biometry::TemplateStore&());
    MOCK_METHOD0(identifier,        biometry::Identifier&());
    MOCK_METHOD0(verifier,          biometry::Verifier&());
};
}

#endif // TESTING_MOCK_DEVICE_H_
