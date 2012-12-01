import pyshiva as ps

w = ps.Window()
paddle = ps.Rect(0,0,20,100)
w.add(paddle)

while w.is_open():
	x,y = ps.get_mouse_pos()
	paddle.y = (w.height - y) - paddle.height/2
	w.refresh()