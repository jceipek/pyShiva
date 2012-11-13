from distutils.core import setup, Extension
 
module1 = Extension('rect', 
	include_dirs = ['.','../include','/usr/local/include'],
	extra_compile_args = ['-lglut', '-lGl', '-lGLU'],
	sources = ['test_rect.c','pyShivamodule.c'],
	)
#module2 = Extension('vg', sources = ['../include/vg/openvg.h'])
 
setup (name = 'PackageName',
        version = '1.0',
        description = 'This is a demo package',
        ext_modules = [module1])
