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

#ifndef BIOMETRYD_CMDS_HELP_H_
#define BIOMETRYD_CMDS_HELP_H_

#include <biometry/util/cli.h>

#include <functional>
#include <iostream>
#include <memory>

namespace biometry
{
namespace cmds
{
class Help : public util::cli::Command
{
public:
    typedef std::function<void(const Command::Ptr&)> Enumerator;
    typedef std::function<void(const Enumerator&)> CommandEnumerator;

    Help(const CommandEnumerator& enumerator);

private:
    CommandEnumerator enumerator;
};
}
}

#endif // BIOMETRYD_CMDS_HELP_H_
