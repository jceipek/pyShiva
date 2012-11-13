from __future__ import print_function

class Entity(object):

    def __init__(self, color, form):
        self.color = color
        self.form = form
    def getColor(self):
        print(self.color)
        return self.color
    def setColor(self, color):
        self.color = color
    def getForm(self):
        print(self.form)
        return self.form


class Path(Entity):
    def __init__(self, color, x, y):
        Entity.__init__(self, color, "Path")    
        self.pos = [x, y]
    def getPos(self):
        print(self.pos)
        return self.pos
    def moveAbs(self, x, y):
        self.pos = [x,y]
        print(self.pos)
        return self.pos
    def moveRel(self, dx, dy):
        self.pos = [self.pos[0]+dx, self.pos[1]+dy]
        print(self.pos)
        return self.pos

class Rectangle(Path):
    def __init__(self, color, x, y, w, h):
        Path.__init__(self, color, x, y)
        self.x = x
        self.y = y
        self.w = w
        self.h = h
        self.UL = [self.x,self.y]
        self.LL = [self.x, self.y-self.h]
        self.UR = [self.x+self.w, self.y]
        self.LR = [self.x+self.w, self.y-self.h]

class Window(object):
    def __init__(self, h, w):
        self.height = h
        self.width = w

entity1 = Entity("red", "none")
entity1.getColor()
entity1.setColor("green")
entity1.getColor()
print("enitity testing completed")
path1 = Path("blue", 3, 4)
path1.getPos()
path1.moveAbs(4,4)
path1.getPos()
path1.moveRel(5,5)
path1.getPos()
path1.getColor()
path1.getForm()

print("entity testing completed")
print("Rectangle test not complete because tests are not written")
