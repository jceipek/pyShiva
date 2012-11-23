pyShiva
=======

The PyShiva project is an experiment in bringing lightning fast high quality 2d graphics to python via the `pyshiva` module.
It aims to provide a simple vector-based API on top of the OpenGL-powered ShivaVG library.

![Demo](https://raw.github.com/jceipek/pyShiva/master/screenshots/ManyObjectsDemo.png "pyShiva Many Objects Demo")

Status
-------
Currently under heavy development.


Full Documentation and Implementation Spec
-------------------------------------------
[Read it here](https://docs.google.com/document/d/1qF8y-nfQE38GGET-e_7vlTTwCGvvnqZUdMKl-cHi_js/edit). You can edit if if you are a collaborator.

Simple Example
---------------
This is how easy it is to write a complete pyShiva program!

	# Import the python module
	import pyshiva as ps

	# Create a window with a title
	win = ps.Window("My First PyShiva Project!", width=640, height=480)
	
	# Create a rectangle at position (10,10) with a width of 100 and height of 50
	r = ps.Rect(10, 10, 100, 50)

	# Add the rectangle to the window
	win.add(r)

	# Until the user presses the close button of the window...
	while win.is_open():
		# ...update the screen and respond to events. Before the first time you do this, the rectangle won't be visible.
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
Without this, you won't be able to run the python demos, make your own projects, or contribute to pyShiva's development.

	sudo apt-get install python-dev
	sudo apt-get install cython

	cd wrapper
	python setup.py build_ext --inplace


Testing the python wrapper
------------------------------------
This demo creates lots of bouncing colored squares.

	cd wrapper
	python many_objects_test.py