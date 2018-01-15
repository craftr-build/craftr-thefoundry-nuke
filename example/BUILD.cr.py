import {glob, path} from 'craftr'
import cxx from 'craftr/lang/cxx'
import 'craftr-foundry-nuke'

cxx.embed(
  name = 'kernels',
  files = ['kernel/saturation.blink'],
  names = ['SaturationKernel']
)

cxx.library(
  name = 'plugin',
  deps = ['//craftr-foundry-nuke:nuke'],
  outname = 'myplugin$(ext)',
  preferred_linkage = 'shared',
  srcs = ['Saturation.cpp'],
  static_runtime = False
)
