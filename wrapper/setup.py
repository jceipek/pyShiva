from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext

import os, sys

libraries = list() #['OpenVG']
extra_objects = list()

if sys.platform.startswith('darwin'):
    os.environ['LDFLAGS'] = '-g -framework OpenGL -framework Cocoa ../glfw-2.7.7/lib/cocoa/libglfw.a ../ShivaVG/src/libOpenVG.a'

elif sys.platform.startswith('linux'):
    libraries.extend(['GL'])

    extra_objects.extend([
        '../glfw-2.7.7/lib/x11/libglfw.a',
        '../ShivaVG/src/libOpenVG.a'])


 
pyshiva_module = Extension('pyshiva',
    include_dirs = ['../ShivaVG/include'],
    sources = ['pyshiva.pyx', 'shiva_wrapper.c'],
    libraries = libraries,
    extra_objects = extra_objects)

setup ( cmdclass = {'build_ext': build_ext},
        name = 'pyshiva',
        version = '1.0',
        description = 'Test package for pyShiva',
        ext_modules = [pyshiva_module])
