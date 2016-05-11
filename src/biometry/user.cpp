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

#include <biometry/user.h>

#include <iostream>

#include <unistd.h>

const biometry::User& biometry::User::root()
{
    static const biometry::User instance{0};
    return instance;
}

const biometry::User& biometry::User::current()
{
    static const biometry::User instance{::getuid()};
    return instance;
}

biometry::User::User(uid_t id) : id{id}
{
}

bool biometry::operator<(const User& lhs, const User& rhs)
{
    return lhs.id < rhs.id;
}

bool biometry::operator==(const User& lhs, const User& rhs)
{
    return lhs.id == rhs.id;
}

std::ostream& biometry::operator<<(std::ostream& out, const User& user)
{
    return out << "User[" << user.id << "]";
}

std::istream& biometry::operator>>(std::istream& in, User& user)
{
    return in >> user.id;
}
