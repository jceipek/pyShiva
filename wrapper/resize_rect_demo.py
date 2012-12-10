#
# Resize Rectangle Demo
#

# Import the python module
import pyshiva as ps
import math, random

# Create a window with the title "Tracker"
w = ps.Window(title = "Tracker")

rs = 50 #rectangle size

# Create 1000 squares with different colors
for i in range(1700/rs):
	for n in range(1000/rs):
		#print "Initialized"
		c = random.random()
		a = abs(math.cos(i))*0.5
		side_length = 1*rs
		r = ps.Rect(i*rs,n*rs,side_length,side_length,(0,1,1,1))
		w.add(r) # Add the rectangles to the window
	
k = 0.25
while w.is_open():
	x,y = ps.get_mouse_pos()
	t = w.s_since_open()
	for (i,r) in enumerate(w):
		#change dimensions of rects based on proximity to cursor
		r.width = rs/(2**(((abs(x-r.x-rs/2)**2+abs(y-r.y-rs/2)**2)**0.5)/(rs**1.35)))
		r.height = rs/(2**(((abs(x-r.x-rs/2)**2+abs(y-r.y-rs/2)**2)**0.5)/(rs**1.35)))
			
	w.refresh()
