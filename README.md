pyShiva
=======

The PyShiva project is an attempt to bring lightning fast high quality 2d graphics to python via the `pyshiva` module.
It aims to provide a simple vector API on top of the OpenGL-powered ShivaVG library.

Status
-------
Currently under heavy development.


Full Documentation and Implementation Spec
-------------------------------------------
[Read it here](https://docs.google.com/document/d/1qF8y-nfQE38GGET-e_7vlTTwCGvvnqZUdMKl-cHi_js/edit). You can edit if if you are a collaborator.

Simple Example
---------------

	import pyshiva as ps
	win = ps.Window("My First PyShiva Project!", width=640, height=480)
	
	# Note: This part doesn't work yet
	# r = ps.Rect(10,10,100,50)
	# win.add(r)

	while win.is_open():
		win.refresh()

Compiling ShivaVG on Ubuntu
----------------------------
	sudo apt-get install libx11-dev
	sudo apt-get install mesa-common-dev
	sudo apt-get install libglu1-mesa-dev

	cd glfw-2.7.7
	make x11

	sudo apt-get install cmake
	sudo apt-get install freeglut3-dev # Required for the examples in ShivaVG
	sudo apt-get install libjpeg-dev # Required for the examples in ShivaVG
	sudo apt-get install libxmu-dev libxi-dev
	sudo apt-get install g++ # Currently required until the CMake system is improved

	cd ..

	cd ShivaVG
	cmake .
	make

	cd ..

	cd wrapper
	make

Building the python wrapper on Ubuntu
--------------------------------------
	sudo apt-get install python-dev
	sudo apt-get install cython

	cd wrapper
	python setup.py build_ext


Testing the python wrapper
------------------------------------
	cd wrapper
	cd build/lib*
	python
	>> import pyshiva as ps
	>> w = ps.Window("Hello, pyShiva!")
	>> w.refresh()
	>> w.x = 100
	>> w.refresh()