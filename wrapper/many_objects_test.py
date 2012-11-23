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
		r = random.random()
		g = random.random()
		b = random.random()
		a = random.random()
		col = ps.Color(r, g, b, a)
		ps.Rect.__init__(self, x, y, 40, 40, col)

	def simulate(self):
		delta_x = self.dir_x * self.speed
		delta_y = self.dir_y * self.speed
		if self.x + delta_x < 0:
			self.dir_x = 1
			self.x = 0
		elif self.x + delta_x >= self.window.width - self.width:
			self.dir_x = -1
			self.x = self.window.width - self.width
		else:
			self.x += delta_x
		if self.y + delta_y < 0:
			self.dir_y = 1
			self.y = 0
		if self.y + delta_y >= self.window.height - self.height:
			self.dir_y = -1
			self.y = self.window.height - self.height
		else:
			self.y += delta_y

w = ps.Window("Many Objects Demo!")

bouncers = list()
count = 0

while w.is_open():
	count += 1
	if count % 2 == 0:
		b = Bouncer(w)
		bouncers.append(b)
		w.add(b)
	for b in bouncers:
		b.simulate()

	w.refresh()