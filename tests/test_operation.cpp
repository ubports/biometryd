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

#include <biometry/operation.h>

#include <gtest/gtest.h>

#include <type_traits>

TEST(Operation, surfaces_correct_types)
{
    struct Test
    {
        typedef bool Progress;
        typedef char Reason;
        typedef short Error;
        typedef int Result;
    };

    static_assert(std::is_same<bool,    typename biometry::Operation<Test>::Progress>::value,   "");
    static_assert(std::is_same<char,    typename biometry::Operation<Test>::Reason>::value,     "");
    static_assert(std::is_same<short,   typename biometry::Operation<Test>::Error>::value,      "");
    static_assert(std::is_same<int,     typename biometry::Operation<Test>::Result>::value,     "");
}
