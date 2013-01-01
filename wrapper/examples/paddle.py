import pyshiva as ps
import random

class Paddle(ps.Rect):
	def __init__(self, y, window):
		self.window = window
		ps.Rect.__init__(self, 50, y+50, 20, 100, (0,0,1,1))
		window.add(self)

	def simulate(self, t):
		x,y = ps.get_mouse_pos()
		self.y = y - self.height/2

class Enemy(ps.Rect):
	def __init__(self, y, window):
		self.window = window
		self.speed = 100
		ps.Rect.__init__(self, window.width-70, y+50, 20, 100, (1,0,0,1))
		window.add(self)


	def simulate(self, t, ypos):
		#x,y = ps.get_mouse_pos()
		self.y  = ypos - self.height/2



class Ball(ps.Rect):
	def __init__(self, window):
		self.window = window
		self.dir_x = 1
		self.dir_y = 1
		self.speed = 500
		ps.Rect.__init__(self, window.height/2, window.width/2, 20, 20)
		window.add(self)

	def change_color(self):
		self.color = (random.random(),random.random(), random.random())

	def simulate(self, t, colliders):
		delta_x = self.dir_x * self.speed * t
		delta_y = self.dir_y * self.speed * t
		for c in colliders:
			if (self.x + delta_x < c.x+c.width < self.x) or (self.x + self.width < c.x < self.x + self.width + delta_x):
				if c.y - self.height/2 < self.y + delta_y < c.y + c.height + self.height/2:
					self.dir_x *= -1
					self.change_color()
					return
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

w = ps.Window()

paddle = Paddle(w.height/2, w)
enemy_paddle = Enemy(w.height/2, w)
ball = Ball(w)

colliders = list()
colliders.append(paddle)
colliders.append(enemy_paddle)

while w.is_open():
	t = w.s_since_refresh()
	paddle.simulate(t)
	enemy_paddle.simulate(t, ball.y)
	ball.simulate(t, colliders)
	w.refresh()
