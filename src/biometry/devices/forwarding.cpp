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

#include <stdexcept>

biometry::devices::Forwarding::Forwarding(const std::shared_ptr<Device>& device)
    : impl{device}
{
    if (not impl)
        throw std::runtime_error{"Cannot construct Forwarding device for null impl."};
}

biometry::TemplateStore& biometry::devices::Forwarding::template_store()
{
    return impl->template_store();
}

biometry::Identifier& biometry::devices::Forwarding::identifier()
{
    return impl->identifier();
}

biometry::Verifier& biometry::devices::Forwarding::verifier()
{
    return impl->verifier();
}
