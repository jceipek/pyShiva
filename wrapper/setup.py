from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext

import os, sys

libraries = ['OpenVG', 'glfw']
library_dirs = ['../ShivaVG/src']

if sys.platform.startswith('darwin'):
    os.environ['LDFLAGS'] = '-g -framework OpenGL -framework Cocoa'
    library_dirs.extend('../glfw-2.7.7/lib/cocoa')

elif sys.platform.startswith('linux'):
    libraries.extend(['GL', 'GLU'])
    #library_dirs.extend('../glfw-2.7.7/lib/x11')
    os.environ['LDFLAGS'] = "-L ../glfw-2.7.7/lib/x11"
 
pyshiva_module = Extension('pyshiva',
    include_dirs = ['../ShivaVG/include', '../glfw-2.7.7/include/GL'],
    sources = ['pyshiva.pyx', 'shiva_wrapper.c'],
    libraries = libraries,
    library_dirs = library_dirs)

setup ( cmdclass = {'build_ext': build_ext},
        name = 'pyshiva',
        version = '1.0',
        description = 'Test package for pyShiva',
        ext_modules = [pyshiva_module])
