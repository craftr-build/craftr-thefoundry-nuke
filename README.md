## @craftr/thefoundry-nuke

Build script to compile plugins for The Foundry's NUKE.

__Windows__

* Requires [Visual Studio 2010](https://www.visualstudio.com/vs/older-downloads/)
  &ndash; Accessible after joining the Visual Studio Dev Essentials (free). You
  need a version that comes with an x64 compiler (eg. Premium or Pro).

### Example build script

```python
import {glob, path} from 'craftr'
import cxx from '@craftr/cxx'
import '@craftr/thefoundry-nuke'

cxx.library(
  name = 'plugin',
  deps = ['//@craftr/thefoundry-nuke:nuke'],
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
