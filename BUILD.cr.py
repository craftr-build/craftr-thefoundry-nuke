
import craftr, {path} from 'craftr'
import cxx from '@craftr/cxx'

nuke_install_dir = craftr.options['nuke.install_dir']

cxx.prebuilt(
  name = 'nuke',
  includes = [
    path.join(nuke_install_dir, 'include'),
    path.join(nuke_install_dir, 'include', 'python-2.7.3')
  ],
  libpath = [
    nuke_install_dir
  ],
  syslibs = [
    'DDImage',
    'glew32',
    'libcurl_imp',
    'libeay32',
    'python27',
    'RIPFramework',
    'ssleay32'
  ]
)
