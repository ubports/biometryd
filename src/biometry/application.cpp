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

#include <biometry/application.h>

biometry::Application biometry::Application::system()
{
    return Application{"system"};
}

biometry::Application::Application() : value{"system"}
{
}

biometry::Application::Application(const std::string& s) : value{s}
{
}

const std::string& biometry::Application::as_string() const
{
    return value;
}

biometry::Application::operator std::string() const
{
    return value;
}

bool biometry::operator==(const biometry::Application& lhs, const biometry::Application& rhs)
{
    return lhs.as_string() == rhs.as_string();
}
