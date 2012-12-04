import pyshiva as ps

w = ps.Window(b"Groups, Yo!")

g = ps.Group(10,10)

g2 = ps.Group(10,10)

r1 = ps.Rect(0,0,100,100, (1,0,0))
r2 = ps.Rect(25,25,50,50, (0,0,1))

g.add(r1)
g.add(r2)

r3 = ps.Rect(30,30,10,10, (0,1,0))
r4 = ps.Rect(50,50,10,10, (0,1,0))

g2.add(r3)
g2.add(r4)

g.add(g2)

w.add(g)

while w.is_open():
	x,y = ps.get_mouse_pos()
	g.x = x
	g.y = y
	w.refresh()