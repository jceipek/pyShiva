import pyshiva as ps

w = ps.Window("Path Test")

p = ps.Path(100,100,color=(1,0,0,1))
p.add_line_to(0,100)
p.add_line_to(100,100)
p.close_path()

w.add(p)

while w.is_open():

	w.refresh()