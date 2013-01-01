#
# Colorful Rose Curves Demo
#

# Import the python module
import pyshiva as ps
import math, random

w = ps.Window(title = "Colorful Rose Curves")

# Create 1000 circles with different colors
for i in range(1000):
    r = random.random()
    a = abs(math.cos(i))*0.5
    radius = abs(math.sin(i))*25
    c = ps.Circle(x = 0, y = 0, radius = radius, color = (r,abs(math.sin(i)),1,0.1))
    w.add(c) # Add the circles to the window.

k = 0.25 # k is the type of rose curve
while w.is_open():
    x,y = ps.get_mouse_pos()
    t = w.s_since_open()*2 # Use a scaled time since program start as the parametric time value
    radius = abs(math.sin(w.s_since_open()))
    if radius < 0.01: # Every time the curve collapses...
        k = random.random() # Randomize the k value to change the type of the curve

    # Place every circle along a rose curve, offset by its index
    for (i,c) in enumerate(w):
        ran = random.random()
        c.x = radius*math.cos(k*(t+i))*math.sin(t+i)*w.width/2+w.width/2
        c.y = radius*math.sin(k*(t+i))*math.sin(t+i)*w.height/2+w.height/2
        c.color.values = (math.cos(t/10.0), math.sin(t/10.0), math.sin(ran*i)/2.0 + .5, 0.1)

    # Update the screen
    w.refresh()