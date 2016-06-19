# Extending Biometryd

Biometryd can be extended by implementing the interface
`biometry::Device`. We support both in-tree and out-of-tree
plugins. In-tree plugin authors should add their device implementation
in the folder `${BIOMETRYD_ROOT}/src/biometry/devices` and submit
their code contribution as a merge proposal to `https://launchpad.net/biometryd`.

Out-of-tree plugin authors should rely on
 - `BIOMETRYD_DEVICES_PLUGIN_DESCRIBE(name, author, desc, major, minor, patch)`
   - `name`        The name of the plugin
   - `author`      The author of the plugin
   - `desc` Human-readable description of the plugin
   - `major`       Major revision of the plugin
   - `minor`       Minor revision of the plugin
   - `patch`       Patch level of the plugin
 - `BIOMETRYD_DEVICES_PLUGIN_CREATE`
 - `BIOMETRYD_DEVICES_PLUGIN_DESTROY`
 
to describe, instantiate and destroy their plugin, respectively. The
following snippet demonstrates a complete plugin definition. The
resulting shared object file should be installed to `biometryd config
--flag default_plugin_directory`. Once the plugin is installed, it can
be referenced by its name as passed to
`BIOMETRYD_DEVICES_PLUGIN_DESCRIBE`.

```c++
#include <biometry/devices/plugin/interface.h>

#include "mock_device.h"

/// [Defining the create function]
BIOMETRYD_DEVICES_PLUGIN_CREATE
{
    return new testing::MockDevice();
}
/// [Defining the create function]

/// [Defining the destroy function]
BIOMETRYD_DEVICES_PLUGIN_DESTROY
{
    delete d;
}
/// [Defining the destroy function]

/// [Describing the plugin]
BIOMETRYD_DEVICES_PLUGIN_DESCRIBE(
	"TestPlugin", 
	"Thomas Voß <thomas.voss@canonical.com>", 
	"Just a plugin for testing purposes", 
	0,
	0,
	0)
/// [Describing the plugin]
 ```
