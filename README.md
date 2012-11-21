PyShiva
=======

The PyShiva project is an attempt to bring lightning fast high quality 2d graphics to python via the `pyshiva` module.
It aims to provide a simple vector API on top of the OpenGL-powered ShivaVG library.

Status
-------
Currently under heavy development.


Simple Example
---------------
Note: This doesn't work yet!

	import pyshiva as ps
	win = ps.Window("My First PyShiva Project!", width=640, height=480)
	r = ps.Rect(10,10,100,50)
	win.add(r)

	running = True
	while running:
		win.refresh()
		if ps.key_pressed("ESC") or not win.is_open:
			running = False

Installing on Ubuntu
---------------------
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

	cd PyShiva
	cmake .
	make

	cd ..

	cd wrapper
	make

