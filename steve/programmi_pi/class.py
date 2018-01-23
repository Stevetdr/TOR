class Volume :
    """ to calculate the volume """
    def __init__(self, area = 1, altezza = 1):
        self.area = area                # attributi di classe
        self.altezza = altezza          # attributi di classe

    def getAltezza(self):
        return self.altezza

    def calcVolume(self):
        return int(self.area) * int(self.altezza)
# . . . . . . . . . . .  . . .  .. . . . . . . . .
class Rectangle :       # Rectangle.
    """ This is Rectangle class"""

    # Method to create object (Constructor)
    def __init__(self, width = 1, high = 1):
        self.width = width
        self.high = high

    def getWidth(self):
        return self.width

    def getHeight(self):
        return self.high

    def getArea(self):    # Method to calculate Area.
        return int(self.width) * int(self.high)

    def calcPerimeter(self):
        return int(self.width) * 2 + int(self.high) * 2
# ========================================================
#programma
high  =   input('Inserisci il lato 1 : ')
width =   input('Inserisci il lato 2 : ')
altezza = input('Inserisci l altezza : ')

r1 = Rectangle(high, width)
area= int(r1.getArea())
q1 = Volume(area, altezza)

print ("r1.width          = ", r1.width)
print ("r1.height         = ", r1.high)
print ("r1.getWidth()     = ", r1.getWidth())
print ("r1.getArea()      = ", r1.getArea())
print ("perimetro         = ", r1.calcPerimeter())
print ("altezza del solido= ", q1.getAltezza())
print ("Volume del solido = " , q1.calcVolume())
print ("-----------------")
print ("doc di rettangolo: ",r1.__doc__)
print ("doc di volume    : ",q1.__doc__)