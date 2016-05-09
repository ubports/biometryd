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

#include <biometry/device_registry.h>
#include <biometry/cmds/list_devices.h>

#include <iostream>

namespace cli = biometry::util::cli;

biometry::cmds::ListDevices::ListDevices()
    : Command{{Name{"list-devices"}, Usage{"list-devices"}, Description{"lists all known devices"}, {}}, []()
      {
          std::cout << "Known devices:" << std::endl;
          for (const auto& pair : biometry::device_registry())
              std::cout << " - " << pair.first << "\t" << pair.second->description() << std::endl;
          return 0;
      }}
{
}
