import random
import time

try:
	import pyshiva as ps
except ImportError as e:
	print("""For now, this program must be run from the same 
directory in which pyshiva.so is located! - Julian""")
	raise e

if __name__ == "__main__":
	width = 1680
	direction = 1
	speed = 1000000
	win = ps.Window("My First pyShiva Program", width = 500, height = 500)

	last_time = time.clock()
	while win.is_open():
		elapsed = time.clock()-last_time
		"""
		win.x += elapsed*speed*direction
		if win.x + win.width >= width or win.x <= 0:
			direction *= -1
		"""

		win.refresh()
		last_time = time.clock()
