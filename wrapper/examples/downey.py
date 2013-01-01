import pyshiva as ps
from PIL import Image
import random
import time
import math

sizex = 60
sizey = 45
timeconstant = 3.0
time_start = time.time()

class Jiggler(ps.Rect):
	def __init__(self, window, x, y, (r,g,b)):
		self.window = window
		self.center = (x*(window.width/sizex), y*(window.width/sizey))
		self.start = (random.random() * window.width, random.random() * window.height)
		ps.Rect.__init__(self, self.center[0], self.center[1], (window.width/sizex)*2, (window.width/sizey)*2, ps.Color(r/255.0,g/255.0,b/255.0,0.75))
	def jiggle(self):
		elapsed = time.time() - time_start
		lerp = elapsed / 3.0
		cx = self.start[0] + (self.center[0] - self.start[0]) * min(lerp, 1.0)
		cy = self.start[1] + (self.center[1] - self.start[1]) * min(lerp, 1.0)
		jigglitude = math.exp(-(elapsed - timeconstant)/timeconstant) + math.pow(math.sin(elapsed*2*math.pi/13.0),32)*32
		theta = time.time() / timeconstant * 2 * math.pi
		x = cx + (random.random()-0.5)*32*jigglitude
		y = cy + (random.random()-0.5)*32*jigglitude
		x = max(0, min(x, self.window.width))
		y = max(0, min(y, self.window.height))
		self.x = (x - self.window.width / 2) * (math.pow(math.sin(elapsed*2*math.pi/20),48)+1) + self.window.width / 2
		self.y = (y - self.window.height / 2) * (math.pow(math.sin(elapsed*2*math.pi/20),48)+1) + self.window.height / 2
		#self.


im = Image.open("downey.jpg")
im.load()
im.thumbnail((sizex, sizey))

jigglers = []
sizex, sizey = im.size

win = ps.Window("THE JPEGGLER", width=sizex*10, height=sizey*10)

for x in range(sizex):
	for y in range(sizey):
		jigglers.append(Jiggler(win, x, sizey-y-1, im.getpixel((x, y))))
		win.add(jigglers[-1])

print 'READY!'
print win.width, win.height
while win.is_open():
	for j in jigglers:
		j.jiggle()
	win.refresh()