from distutils.core import setup, Extension
 
module1 = Extension('rect', 
	include_dirs = ['.','../include','/usr/local/include'],
	sources = ['test_rect.c','pyShivamodule.c'],
	libraries = ['OpenVG', 'glut', 'GL', 'GLU'],
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
#module2 = Extension('vg', sources = ['../include/vg/openvg.h'])
 
setup (name = 'PackageName',
        version = '1.0',
        description = 'This is a demo package',
        ext_modules = [module1])
