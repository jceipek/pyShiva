import pyshiva as ps
from random import random

w = ps.Window()

r = ps.Rect(x=100, y=100, width=100, height=100)

w.add(r)

while w.is_open():
	r.width = random()*500
	r.color = (random(),random(),random())
	w.refresh()