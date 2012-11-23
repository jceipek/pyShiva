import pyshiva as ps
import random

class Bouncer(ps.Rect):
	def __init__(self, window):
		self.window = window
		self.dir_x = 1
		self.dir_y = 1
		self.speed = random.random()*10
		x = random.randint(0, self.window.width)
		y = random.randint(0, self.window.height)
		ps.Rect.__init__(self,x,y,20,20)

	def simulate(self):
		if self.x < 0:
			self.dir_x = 1
		elif self.x > self.window.width:
			self.dir_x = -1
		if self.y < 0:
			self.dir_y = 1
		elif self.y > self.window.height:
			self.dir_y = -1
		self.x += self.dir_x * self.speed
		self.y += self.dir_y * self.speed

w = ps.Window("Hello, World!", width = 1000, height = 1000)

bouncers = list()
count = 0

while w.is_open():
	count += 1
	if count % 100 == 0:
		b = Bouncer(w)
		bouncers.append(b)
		w.add(b)
	for b in bouncers:
		b.simulate()

	w.refresh()