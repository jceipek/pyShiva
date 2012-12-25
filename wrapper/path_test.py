import pyshiva as ps
from random import random
import math

w = ps.Window("Path Test")

p = ps.Path(100,100)#,color=(1,0,0,1))
p.add_line_to(0,100)
p.add_line_to(100,100)
#p.close()
w.add(p)

#p1 = ps.Path(200,100, stroke_thickness=3, color=(1,0,0))
#p1.add_quad_to(0,100,100,100)
#p1.add_quad_to(0,-100,-100,100, is_absolute=False)
#w.add(p1)


while w.is_open():
	t = w.s_since_open()
	p[0].x = math.sin(t)*100

	w.refresh()