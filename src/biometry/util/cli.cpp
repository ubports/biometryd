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

#include <biometry/util/cli.h>

namespace cli = biometry::util::cli;

cli::Command::Info cli::Command::info() const
{
    return info_;
}

int cli::Command::run()
{
    return run_();
}

cli::Command::Command(const cli::Command::Info& info, const std::function<int()>& run)
    : info_(info),
      run_{run}
{
}

cli::Command::Info& cli::Command::mutable_info()
{
    return info_;
}

std::function<int()>& cli::Command::mutable_run()
{
    return run_;
}
