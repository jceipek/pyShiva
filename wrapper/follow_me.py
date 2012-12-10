import pyshiva as ps
from math import sin

w = ps.Window("Follow Me!")

for e in range(50):
	color = (abs(sin(e)), 0, abs(sin(e)), 0.5)
	w.add(ps.Ellipse(0, 0, e, e, color = color))

while w.is_open():
	t = w.s_since_refresh()
	x,y = ps.get_mouse_pos()
	for i,e in enumerate(w):
		dx = x - e.x
		dy = y - e.y
		e.x += dx*(t*i*0.3)
		e.y += dy*(t*i*0.3)

	w.refresh()