from distutils.core import setup, Extension

import os, sys

libraries = ['OpenVG']
if sys.platform.startswith('darwin'):
	os.environ['LDFLAGS'] = '-g -framework GLUT -framework OpenGL -framework Cocoa'
elif sys.platform.startswith('linux'):
	libraries.extend(['glut', 'GL', 'GLU'])

 
pyshiva_module = Extension('rect', 
	include_dirs = ['.','../include','/usr/local/include'],
	sources = ['test_rect.c','pyShivamodule.c'],
	libraries = libraries,
	extra_objects = [
	'libOpenVG_la-shArrays.o',
	'libOpenVG_la-shContext.o',
	'libOpenVG_la-shExtensions.o',
	'libOpenVG_la-shGeometry.o',
	'libOpenVG_la-shImage.o',
	'libOpenVG_la-shPaint.o',
	'libOpenVG_la-shParams.o',
	'libOpenVG_la-shPath.o',
	'libOpenVG_la-shPipeline.o',
	'libOpenVG_la-shVectors.o',
	'libOpenVG_la-shVgu.o'])
 
setup (name = 'PackageName',
        version = '1.0',
        description = 'This is a demo package',
        ext_modules = [pyshiva_module])
