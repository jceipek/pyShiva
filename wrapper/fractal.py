'''
Fractal Demo.
'''

import pyshiva as ps
from math import sin, cos

w = ps.Window(title = "Fractals!")

def create_circles(iterations, x, y, radius):
	w.add(ps.Circle(x, y, radius,
					color = (abs(sin(radius)),0,abs(cos(radius)),0.2)))
	if iterations > 0:
		iterations -= 1
		factor = 2.4
		create_circles(iterations, x, y + radius/factor, radius/factor)
		create_circles(iterations, x, y - radius/factor, radius/factor)
		create_circles(iterations, x + radius/factor, y, radius/factor)
		create_circles(iterations, x - radius/factor, y, radius/factor)

create_circles(5,w.width/2,w.height/2,w.height/2)

while w.is_open():
	w.refresh()