import pyshiva as ps #this lets you use our library, all the modules from pyshiva will start with ps.___

window = ps.Window(title="Play with me!", width=600, height=450) #initialize window with title and dimensions

#set background color to pink
pink = (1,.2,1,1)
window.bg_color = pink


#make a circle called bouncer
cyan = (0, 1, 1, 1) #RGB color tuple with alpha value (opacity)
bouncer = ps.Circle(x=200, y=300, radius=50, color=cyan)

#make a rectangle
blue = (0,0,1,.5)
rect_width = 50
rect_height = 300
rectangle = ps.Rect(x=15, y=0, width=rect_width, height=rect_height, color=blue)

#put shapes into the window
window.add(bouncer)
window.add(rectangle)

speed = 1
x_dir = 1
y_dir = 1

while window.is_open():

	#move bouncer
	bouncer.x = bouncer.x + speed * x_dir
	bouncer.y = bouncer.y + speed * y_dir

	#update rectangle height
	rectangle.height = bouncer.y


	#bounce!
	if bouncer.x - bouncer.radius < 0:
		x_dir = 1
	if bouncer.x + bouncer.radius > window.width:
		x_dir = -1
	if bouncer.y - bouncer.radius < 0:
		y_dir = 1
	if bouncer.y + bouncer.radius > window.height:
		y_dir = -1

	#draw to screen
	window.refresh()