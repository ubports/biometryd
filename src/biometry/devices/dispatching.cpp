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

#include <biometry/identifier.h>
#include <biometry/operation.h>
#include <biometry/template_store.h>

namespace
{
template<typename T>
class DispatchingOperation : public biometry::Operation<T>
{
public:

    DispatchingOperation(const std::shared_ptr<biometry::util::Dispatcher>& dispatcher, const std::shared_ptr<biometry::Operation<T>>& impl)
        : dispatcher{dispatcher},
          impl{impl}
    {
    }

    void start_with_observer(const typename biometry::Operation<T>::Observer::Ptr& observer) override
    {
        auto i= impl;
        dispatcher->dispatch([i, observer]()
        {
            i->start_with_observer(observer);
        });
    }

    void cancel() override
    {
        auto i = impl;
        dispatcher->dispatch([i]()
        {
            i->cancel();
        });
    }

private:
    std::shared_ptr<biometry::util::Dispatcher> dispatcher;
    std::shared_ptr<biometry::Operation<T>> impl;
};
}

biometry::devices::Dispatching::TemplateStore::TemplateStore(const std::shared_ptr<biometry::util::Dispatcher>& dispatcher, const std::shared_ptr<biometry::Device>& impl)
    : dispatcher{dispatcher},
      impl{impl}
{
}

biometry::Operation<biometry::TemplateStore::SizeQuery>::Ptr biometry::devices::Dispatching::TemplateStore::size(const biometry::Application& app, const biometry::User& user)
{
    return std::make_shared<DispatchingOperation<biometry::TemplateStore::SizeQuery>>(dispatcher, impl->template_store().size(app, user));
}

biometry::Operation<biometry::TemplateStore::List>::Ptr biometry::devices::Dispatching::TemplateStore::list(const biometry::Application& app, const biometry::User& user)
{
    return std::make_shared<DispatchingOperation<biometry::TemplateStore::List>>(dispatcher, impl->template_store().list(app, user));
}

biometry::Operation<biometry::TemplateStore::Enrollment>::Ptr biometry::devices::Dispatching::TemplateStore::enroll(const biometry::Application& app, const biometry::User& user)
{
    return std::make_shared<DispatchingOperation<biometry::TemplateStore::Enrollment>>(dispatcher, impl->template_store().enroll(app, user));
}

biometry::Operation<biometry::TemplateStore::Removal>::Ptr biometry::devices::Dispatching::TemplateStore::remove(const biometry::Application& app, const biometry::User& user, biometry::TemplateStore::TemplateId id)
{
    return std::make_shared<DispatchingOperation<biometry::TemplateStore::Removal>>(dispatcher, impl->template_store().remove(app, user, id));
}

biometry::Operation<biometry::TemplateStore::Clearance>::Ptr biometry::devices::Dispatching::TemplateStore::clear(const biometry::Application& app, const biometry::User& user)
{
    return std::make_shared<DispatchingOperation<biometry::TemplateStore::Clearance>>(dispatcher, impl->template_store().clear(app, user));
}

biometry::devices::Dispatching::Identifier::Identifier(const std::shared_ptr<biometry::util::Dispatcher>& dispatcher, const std::shared_ptr<biometry::Device>& impl)
    : dispatcher{dispatcher},
      impl{impl}
{
}

biometry::Operation<biometry::Identification>::Ptr biometry::devices::Dispatching::Identifier::identify_user(const biometry::Application& app, const biometry::Reason& reason)
{
    return std::make_shared<DispatchingOperation<biometry::Identification>>(dispatcher, impl->identifier().identify_user(app, reason));
}

biometry::devices::Dispatching::Verifier::Verifier(const std::shared_ptr<biometry::util::Dispatcher>& dispatcher, const std::shared_ptr<biometry::Device>& impl)
    : dispatcher{dispatcher},
      impl{impl}
{
}

biometry::Operation<biometry::Verification>::Ptr biometry::devices::Dispatching::Verifier::verify_user(const biometry::Application& app, const biometry::User& user, const biometry::Reason& reason)
{
    return std::make_shared<DispatchingOperation<biometry::Verification>>(dispatcher, impl->verifier().verify_user(app, user, reason));
}

biometry::devices::Dispatching::Dispatching(const std::shared_ptr<biometry::util::Dispatcher>& dispatcher, const std::shared_ptr<Device>& device)
    : template_store_{dispatcher, device},
      identifier_{dispatcher, device},
      verifier_{dispatcher, device}
{
}

biometry::TemplateStore& biometry::devices::Dispatching::template_store()
{
    return template_store_;
}

biometry::Identifier& biometry::devices::Dispatching::identifier()
{
    return identifier_;
}

biometry::Verifier& biometry::devices::Dispatching::verifier()
{
    return verifier_;
}
