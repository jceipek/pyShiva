import pyshiva as ps
from math import sin

w = ps.Window(title = "Follow Me!")

for circle_idx in range(25):
	rgba = (abs(sin(circle_idx)), 0, abs(sin(circle_idx)), 0.5)
	w.add(ps.Circle(x = 0, y = 0, radius = circle_idx, color = rgba))

while w.is_open():
	t = w.s_since_refresh()
	x,y = ps.get_mouse_pos()
	for i,e in enumerate(w):
		dx = x - e.x
		dy = y - e.y
		e.x += dx*(t*i*0.3)
		e.y += dy*(t*i*0.3)

	w.refresh()