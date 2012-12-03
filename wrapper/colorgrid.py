#
# Color Grid Demo
#

# Import the python module
import pyshiva as ps
import math, random

# Create a window with the title "Color Grid"
w = ps.Window(title = "Color Grid")

all_rects = list()
all_colors = list()
# Creats a list of 10 colors, ranging from blue to red
for i in range(10):
    testColor = (1-.1*i,0,.1*i,1)
    all_colors.append(testColor)

#creates a 10 by 10 grid of squares, choosing from the list of colors
for x in range(10):
    for y in range(10):
        side_length = 50
        r = ps.Rect(x*(side_length+1),y*(side_length+1),side_length,side_length,all_colors[abs(x-y)])
        w.add(r) # Add the rectangles to the window...
        all_rects.append(r) # and keep track of them with a list

#changes which color each square is based on the time.  This means swapping out colors, not creating new colors.
oldt = 0
while w.is_open():
    t = w.s_since_open()
    for x in range(10):
        for y in range(10):
            r = all_rects[y*10+x]
            index = (x-y+int(t))%10
            r.color = all_colors[index]
    oldt = t

    # Update the screen
    
    w.refresh()