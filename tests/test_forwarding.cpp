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

#include <biometry/devices/forwarding.h>

#include <gmock/gmock.h>

#include "mock_device.h"

TEST(Forwarding, calls_into_implementation)
{
    using namespace ::testing;

    MockTemplateStore ts;
    EXPECT_CALL(ts, size(_, _)).Times(1).WillOnce(Return(biometry::Operation<biometry::TemplateStore::SizeQuery>::Ptr{}));
    EXPECT_CALL(ts, enroll(_, _)).Times(1).WillOnce(Return(biometry::Operation<biometry::TemplateStore::Enrollment>::Ptr{}));
    EXPECT_CALL(ts, clear(_, _)).Times(1).WillOnce(Return(biometry::Operation<biometry::TemplateStore::Clearance>::Ptr{}));

    MockIdentifier identifier;
    EXPECT_CALL(identifier, identify_user(_, _)).Times(1).WillOnce(Return(biometry::Operation<biometry::Identification>::Ptr{}));

    MockVerifier verifier;
    EXPECT_CALL(verifier, verify_user(_, _, _)).Times(1).WillOnce(Return(biometry::Operation<biometry::Verification>::Ptr{}));

    auto impl = std::make_shared<MockDevice>();
    EXPECT_CALL(*impl, template_store()).Times(3).WillRepeatedly(ReturnRef(ts));
    EXPECT_CALL(*impl, identifier()).Times(1).WillOnce(ReturnRef(identifier));
    EXPECT_CALL(*impl, verifier()).Times(1).WillOnce(ReturnRef(verifier));

    const biometry::Application app{biometry::Application::system()};
    const biometry::Reason reason{biometry::Reason::unknown()};
    const biometry::User user = biometry::User::current();

    impl->template_store().size(app, user);
    impl->template_store().enroll(app, user);
    impl->template_store().clear(app, user);
    impl->identifier().identify_user(app, reason);
    impl->verifier().verify_user(app, user, reason);
}
