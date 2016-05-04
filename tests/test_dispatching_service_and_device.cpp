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

#include <biometry/devices/dispatching.h>

#include "mock_device.h"

#include <gmock/gmock.h>

namespace
{
struct MockDispatcher : public biometry::util::Dispatcher
{
    MOCK_METHOD1(dispatch, void(const Task&));
};
}

TEST(DispatchingDevice, calls_into_dispatcher_for_template_store_size_query)
{
    using namespace testing;
    auto mock_observer = std::make_shared<NiceMock<MockObserver<biometry::TemplateStore::SizeQuery>>>();

    auto template_store = std::make_shared<NiceMock<MockTemplateStore>>();
    ON_CALL(*template_store, size(_, _)).WillByDefault(Return(std::make_shared<NiceMock<MockOperation<biometry::TemplateStore::SizeQuery>>>()));

    auto device = std::make_shared<NiceMock<MockDevice>>();
    ON_CALL(*device, template_store()).WillByDefault(ReturnRef(*template_store));

    auto dispatcher = std::make_shared<NiceMock<MockDispatcher>>();
    EXPECT_CALL(*dispatcher, dispatch(_)).Times(1).WillOnce(Invoke([](const biometry::util::Dispatcher::Task& task) { task(); }));

    auto dispatching = std::make_shared<biometry::devices::Dispatching>(dispatcher, device);
    auto op = dispatching->template_store().size(biometry::Application::system(), biometry::User::current());

    op->start_with_observer(mock_observer);
}

TEST(DispatchingDevice, calls_into_dispatcher_for_template_store_size_enrollment)
{
    using namespace testing;
    auto mock_observer = std::make_shared<NiceMock<MockObserver<biometry::TemplateStore::Enrollment>>>();

    auto template_store = std::make_shared<NiceMock<MockTemplateStore>>();
    ON_CALL(*template_store, enroll(_, _)).WillByDefault(Return(std::make_shared<NiceMock<MockOperation<biometry::TemplateStore::Enrollment>>>()));

    auto device = std::make_shared<NiceMock<MockDevice>>();
    ON_CALL(*device, template_store()).WillByDefault(ReturnRef(*template_store));

    auto dispatcher = std::make_shared<NiceMock<MockDispatcher>>();
    EXPECT_CALL(*dispatcher, dispatch(_)).Times(1).WillOnce(Invoke([](const biometry::util::Dispatcher::Task& task) { task(); }));

    auto dispatching = std::make_shared<biometry::devices::Dispatching>(dispatcher, device);
    auto op = dispatching->template_store().enroll(biometry::Application::system(), biometry::User::current());

    op->start_with_observer(mock_observer);
}

TEST(DispatchingDevice, calls_into_dispatcher_for_template_store_clearance)
{
    using namespace testing;
    auto mock_observer = std::make_shared<NiceMock<MockObserver<biometry::TemplateStore::Clearance>>>();

    auto template_store = std::make_shared<NiceMock<MockTemplateStore>>();
    ON_CALL(*template_store, clear(_, _)).WillByDefault(Return(std::make_shared<NiceMock<MockOperation<biometry::TemplateStore::Clearance>>>()));

    auto device = std::make_shared<NiceMock<MockDevice>>();
    ON_CALL(*device, template_store()).WillByDefault(ReturnRef(*template_store));

    auto dispatcher = std::make_shared<NiceMock<MockDispatcher>>();
    EXPECT_CALL(*dispatcher, dispatch(_)).Times(1).WillOnce(Invoke([](const biometry::util::Dispatcher::Task& task) { task(); }));

    auto dispatching = std::make_shared<biometry::devices::Dispatching>(dispatcher, device);
    auto op = dispatching->template_store().clear(biometry::Application::system(), biometry::User::current());

    op->start_with_observer(mock_observer);
}

TEST(DispatchingDevice, calls_into_dispatcher_for_identification)
{
    using namespace testing;
    auto mock_observer = std::make_shared<NiceMock<MockObserver<biometry::Identification>>>();

    auto identifier = std::make_shared<NiceMock<MockIdentifier>>();
    ON_CALL(*identifier, identify_user(_, _)).WillByDefault(Return(std::make_shared<NiceMock<MockOperation<biometry::Identification>>>()));

    auto device = std::make_shared<NiceMock<MockDevice>>();
    ON_CALL(*device, identifier()).WillByDefault(ReturnRef(*identifier));

    auto dispatcher = std::make_shared<NiceMock<MockDispatcher>>();
    EXPECT_CALL(*dispatcher, dispatch(_)).Times(1).WillOnce(Invoke([](const biometry::util::Dispatcher::Task& task) { task(); }));

    auto dispatching = std::make_shared<biometry::devices::Dispatching>(dispatcher, device);
    auto op = dispatching->identifier().identify_user(biometry::Application::system(), biometry::Reason::unknown());

    op->start_with_observer(mock_observer);
}

