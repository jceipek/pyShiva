#
# Color Change Demo
#

# Import the python module
import pyshiva as ps
import math, random

# Create a window with the title "Color Grid"
w = ps.Window(title = "Color Change Demo")

all_rects = list()

# Creates two color objects
colorA = ps.Color(1,0,0,.5)
colorB = ps.Color(0,1,1,.5) 

#creates a 10 by 10 grid of squares, choosing from the list of colors
side_length = 50
for x in range(10):
    for y in range(10):
        if (y+x)%2 == 0:
            r = ps.Rect(x*(side_length+1),y*(side_length+1),side_length,side_length,colorA)
        else:
            r = ps.Rect(x*(side_length+1),y*(side_length+1),side_length,side_length,colorB)
        w.add(r) # Add the rectangles to the window...
        all_rects.append(r) # and keep track of them with a list

#changes which color each square is based on the time.  This means swapping out colors, not creating new colors.
oldt = 0
while w.is_open():
    t = w.s_since_open()
    if t > oldt+1:
        colorB.g = 1/t
        #colorB.g = 1/t
        oldt = t

    # Update the screen
    
    w.refresh()