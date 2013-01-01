# Copyright (C) 2012 Julian Ceipek, Berit Johnson, Diana Vermilya, Charles Gwennap
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
# 
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
# 
# You should have received a copy of the GNU Lesser General Public
# License along with this library in the file COPYING;
# if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
#

'''
setup.py
Use this file to build the pyshiva Python module:
    python setup.py install
'''

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
        author='Julian Ceipek, Berit Johnson, Diana Vermilya, Charles Gwennap',
        author_email='julian.ceipek@gmail.com',
        url='https://github.com/jceipek/pyShiva/',
        download_url = "https://github.com/jceipek/pyShiva/archive/master.zip",
        keywords = ["vector graphics", "2D", "graphics", "simulation", "visualization"],
        description = 'A simple object-oriented vector graphics library powered by ShivaVG',
        classifiers = [
        "Programming Language :: Cython",
        "Programming Language :: Python :: 3",
        "Programming Language :: Python :: 2",
        "Programming Language :: C",
        "Development Status :: 3 - Alpha",
        "Environment :: X11 Applications",
        "Environment :: MacOS X",
        "Intended Audience :: Developers",
        "Intended Audience :: Science/Research",
        "Intended Audience :: Education",
        "License :: OSI Approved :: GNU Library or Lesser General Public License (LGPL)",
        "Operating System :: MacOS",
        "Operating System :: Unix",
        "Topic :: Software Development :: Libraries :: Python Modules",
        "Topic :: Artistic Software",
        "Topic :: Games/Entertainment",
        "Topic :: Games/Entertainment",
        "Topic :: Scientific/Engineering :: Visualization"
        ],
        long_description = '''\
The PyShiva project is an experiment in bringing lightning fast high quality 
2d graphics to python via the pyshiva module. It aims to provide a simple vector-based 
API on top of the OpenGL-powered ShivaVG library.
'''
        ext_modules = [pyshiva_module])
