import pyshiva as ps
import random

w = ps.Window()

count = 30

data = [random.random() for i in range(count)]

for x in range(count):
	rect_width = w.width/float(count)
	w.add(ps.Rect((rect_width+1)*x,0,rect_width,10, (0,1,0,0.6)))

t_since_last = 0
while w.is_open():
	t = w.s_since_refresh()
	t_since_last += t
	for i,r in enumerate(w):
		n = data[i]
		r.height = n*w.height
	if t_since_last > 0.2:
		del data[0]
		data.append(random.random())
		t_since_last = 0
	w.refresh()