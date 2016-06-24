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

#include <biometry/util/property_store.h>

#include <core/posix/exec.h>

std::string biometry::util::AndroidPropertyStore::get(const std::string& key) const
{
    core::posix::ChildProcess getprop = core::posix::exec("/usr/bin/getprop", {key}, {}, core::posix::StandardStream::stdout);
    std::string value; getprop.cout() >> value;

    auto result = getprop.wait_for(core::posix::wait::Flags::untraced);

    if (result.status == core::posix::wait::Result::Status::exited && result.detail.if_exited.status == core::posix::exit::Status::success)
        return value;

    throw std::out_of_range{key};
}
