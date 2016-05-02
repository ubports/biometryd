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

#include <biometry/runtime.h>

#include <biometry/dbus/skeleton/service.h>
#include <biometry/dbus/stub/service.h>

#include <core/dbus/fixture.h>
#include <core/posix/fork.h>
#include <core/posix/signal.h>

#include <core/dbus/asio/executor.h>

#include <gmock/gmock.h>

#include "did_finish_successfully.h"
#include "mock_device.h"

namespace
{
struct MockService : public biometry::Service
{
    MOCK_CONST_METHOD0(default_device, std::shared_ptr<biometry::Device>());
};

struct TestDbusStubSkeleton : public core::dbus::testing::Fixture
{
    struct StubScope
    {
        ~StubScope()
        {
            bus->stop();
        }

        std::shared_ptr<biometry::Runtime> rt;
        core::dbus::Bus::Ptr bus;
    };

    struct SkeletonScope
    {
        ~SkeletonScope()
        {
            bus->stop();
        }

        core::posix::exit::Status run()
        {
            trap->run();
            return ::testing::Test::HasFailure() ? core::posix::exit::Status::failure : core::posix::exit::Status::success;
        }

        std::shared_ptr<core::posix::SignalTrap> trap;
        std::shared_ptr<biometry::Runtime> rt;
        core::dbus::Bus::Ptr bus;
    };

    std::shared_ptr<SkeletonScope> skeleton_scope()
    {
        auto trap = core::posix::trap_signals_for_all_subsequent_threads({core::posix::Signal::sig_term});
        trap->signal_raised().connect([trap](core::posix::Signal)
        {
            trap->stop();
        });

        auto rt = biometry::Runtime::create();
        auto bus = session_bus();

        bus->install_executor(core::dbus::asio::make_executor(bus, rt->service()));
        rt->start();

        return std::shared_ptr<SkeletonScope>{new SkeletonScope{trap, rt, bus}};
    }

    std::shared_ptr<StubScope> stub_scope()
    {
        auto rt = biometry::Runtime::create();
        auto bus = session_bus();

        bus->install_executor(core::dbus::asio::make_executor(bus, rt->service()));
        rt->start();

        return std::shared_ptr<StubScope>{new StubScope{rt, bus}};
    }
};

template<typename T>
struct MockKeepAliveObserver : public testing::MockObserver<T>
{
    MockKeepAliveObserver(const typename biometry::Operation<T>::Ptr& op) : op{op}
    {
    }

    std::promise<typename testing::MockObserver<T>::Result> promise;
    typename biometry::Operation<T>::Ptr op;
};

template<typename T>
std::future<typename biometry::Operation<T>::Observer::Result> start(const typename biometry::Operation<T>::Ptr& op)
{
    auto observer = new MockKeepAliveObserver<T>{op};
    op->start_with_observer(typename biometry::Operation<T>::Observer::Ptr{observer});
    return observer->promise.get_future();
}

}

TEST_F(TestDbusStubSkeleton, stub_skeleton_service_can_talk_with_one_another)
{
    using namespace ::testing;

    auto skeleton = [this]()
    {
        auto scope = skeleton_scope();

        auto identifier = std::make_shared<NiceMock<MockIdentifier>>();
        ON_CALL(*identifier, identify_user(_,_)).WillByDefault(Return(std::make_shared<MockOperation<biometry::Identification>>()));

        auto template_store = std::make_shared<NiceMock<MockTemplateStore>>();
        ON_CALL(*template_store, size(_, _)).WillByDefault(Return(std::make_shared<MockOperation<biometry::TemplateStore::SizeQuery>>()));
        ON_CALL(*template_store, enroll(_, _)).WillByDefault(Return(std::make_shared<MockOperation<biometry::TemplateStore::Enrollment>>()));
        ON_CALL(*template_store, clear(_, _)).WillByDefault(Return(std::make_shared<MockOperation<biometry::TemplateStore::Clearance>>()));

        auto device = std::make_shared<NiceMock<MockDevice>>();
        ON_CALL(*device, identifier()).WillByDefault(ReturnRef(*identifier));
        ON_CALL(*device, template_store()).WillByDefault(ReturnRef(*template_store));

        auto service = std::make_shared<NiceMock<MockService>>();
        ON_CALL(*service, default_device()).WillByDefault(Return(device));

        auto skeleton = biometry::dbus::skeleton::Service::create_for_bus(scope->bus, service);

        return scope->run();
    };

    auto stub = [this]()
    {
        auto app = biometry::Application::system();
        auto reason = biometry::Reason::unknown();
        auto user = biometry::User::current();

        auto scope = stub_scope();
        auto service = biometry::dbus::stub::Service::create_for_bus(scope->bus);
        auto device = service->default_device();

        auto f1 = start<biometry::TemplateStore::SizeQuery>(device->template_store().size(app, user));
        auto f2 = start<biometry::TemplateStore::Enrollment>(device->template_store().enroll(app, user));
        auto f3 = start<biometry::TemplateStore::Clearance>(device->template_store().clear(app, user));

        auto f4 = start<biometry::Identification>(device->identifier().identify_user(app, reason));

        return ::testing::Test::HasFailure() ? core::posix::exit::Status::failure : core::posix::exit::Status::success;
    };

    auto cp_skeleton = core::posix::fork(skeleton, core::posix::StandardStream::empty);
    // std::cout << cp_skeleton.pid() << std::endl; char c; std::cin >> c;
    std::this_thread::sleep_for(std::chrono::milliseconds{500});
    auto cp_stub = core::posix::fork(stub, core::posix::StandardStream::empty);

    EXPECT_TRUE(did_finish_successfully(cp_stub.wait_for(core::posix::wait::Flags::untraced)));
    ASSERT_NO_THROW(cp_skeleton.send_signal_or_throw(core::posix::Signal::sig_term));
    EXPECT_TRUE(did_finish_successfully(cp_skeleton.wait_for(core::posix::wait::Flags::untraced)));
}
