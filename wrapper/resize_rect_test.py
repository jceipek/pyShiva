#
# Resize Rectangle Demo
#

# Import the python module
import pyshiva as ps
import math, random

# Create a window with the title "Rose Curves"
w = ps.Window(title = "Rose Curves")

all_rects = list()

# Create 1000 squares with different colors
for i in range(1):
	#print "Initialized"
	c = random.random()
	a = abs(math.cos(i))*0.5
	side_length = 1*50
	r = ps.Rect(0,0,side_length,side_length,(c,abs(math.sin(i)),1,1))
	w.add(r) # Add the rectangles to the window...
	all_rects.append(r) # and keep track of them with a list

k = 0.25
while w.is_open():
	t = w.s_since_open()
	for (i,r) in enumerate(all_rects):
		if(int(t)%2):
			r.x = w.width/2
			r.y = w.height/2
			r.width = float(100.0)
			r.height = float(100.0)
			#print "Width: " +str(r.width)
			#print "Height: " +str(r.height)
		else:
			r.x = 0
			r.y = 0
			r.width = float(50.0)
			r.height = float(50.0)
			
	w.refresh()
