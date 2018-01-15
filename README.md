## craftr-foundry-nuke

Build script to compile plugins for The Foundry's NUKE.

__Windows__

* [Visual Studio 2010](https://www.visualstudio.com/vs/older-downloads/)
  &ndash; Accessible after joining the Visual Studio Dev Essentials (free).
  Make sure to download the Premium or Pro versions of Visual Studio as the
  Express version does not come with an x64 compiler.

### Example build script

```python
import {glob, path} from 'craftr'
import cxx from 'craftr/lang/cxx'
import 'craftr-foundry-nuke'

cxx.library(
  name = 'plugin',
  deps = ['//craftr-foundry-nuke:nuke'],
  outname = 'myplugin$(ext)',
  preferred_linkage = 'shared',
  srcs = glob('src/*.cpp'),
  static_runtime = False
)
```

### Example `menu.py` script

```python
import nuke
nuke.pluginAddPath(os.path.expanduser('~/.nuke/plugins'))
nuke.load('myplugin')
nuke.menu('Nodes').addCommand('MyPlugin/MyNode', lambda: nuke.createNode('MyPlugin_MyNode'))
```

### Resources

* [Nuke Developer Kit 10.5 Refeference](https://docs.thefoundry.co.uk/nuke/105/ndkreference/)
  * [Classes](https://docs.thefoundry.co.uk/nuke/105/ndkreference/Plugins/classes.html)
  * [Input Handling](https://www.thefoundry.co.uk/products/nuke/developers/105/ndkdevguide/split-and-execute/input.html)
* [Nuke Python API](https://docs.thefoundry.co.uk/nuke/70/pythonreference/)
* [Nuke Example Plugins](https://docs.thefoundry.co.uk/nuke/63/ndkreference/Plugins/00b.source.html)
* [Blink API Example](https://docs.thefoundry.co.uk/nuke/80/Blink/example_plugin.html)
