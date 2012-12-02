pyShiva
=======

The PyShiva project is an experiment in bringing lightning fast high quality 2d graphics to python via the `pyshiva` module.
It aims to provide a simple vector-based API on top of the OpenGL-powered ShivaVG library.

![Demo](https://raw.github.com/jceipek/pyShiva/master/screenshots/RoseCurves.png "pyShiva Rose Curves")

This is how easy it is to write a complete pyShiva program!

	#
	# Rose Curves Demo
	#

	# Import the python module
	import pyshiva as ps
	import math, random

	# Create a window with the title "Rose Curves"
	w = ps.Window(title = "Rose Curves")

	all_rects = list()

	# Create 1000 squares with different colors
	for i in range(1000):
		r = random.random()
		a = abs(math.cos(i))*0.5
		side_length = abs(math.sin(i))*50
		r = ps.Rect(0,0,side_length,side_length,(r,math.sin(i),math.cos(i),a))
		w.add(r) # Add the rectangles to the window...
		all_rects.append(r) # and keep track of them with a list

	k = 0.25
	while w.is_open():
		t = w.s_since_open()*2 # Use a scaled time since program start as the parametric time value
		radius = abs(math.sin(w.s_since_open()))
		if radius < 0.01: # Every time the curve collapses...
			k = random.random() # Randomize the k value to change the type of the curve
		# Place every rectangle along a rose curve, offset by its index
		for (i,r) in enumerate(all_rects):
			r.x = radius*math.cos(k*(t+i))*math.sin(t+i)*w.width/2+w.width/2
			r.y = radius*math.sin(k*(t+i))*math.sin(t+i)*w.height/2+w.height/2

		# Update the screen
		w.refresh()


Status
-------
Currently under heavy development.


Full Documentation and Implementation Spec
-------------------------------------------
[Read it here](https://docs.google.com/document/d/1qF8y-nfQE38GGET-e_7vlTTwCGvvnqZUdMKl-cHi_js/edit). You can edit if if you are a collaborator.

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