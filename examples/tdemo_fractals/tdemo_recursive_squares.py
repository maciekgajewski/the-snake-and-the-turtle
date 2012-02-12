"""      turtle-example-suite:

        tdemo_recursive squares.py

This program draws a recursive pattern of
coloured squares, that have smaller squares
at their vertices. To each square size
belongs a different colour.

The squares are stamped, the larger ones
first, the smaller ones upon them.

Finally you see 5461 squares.
"""
from qturtle import *
from time import clock

colors = ["red", "green", "blue", "yellow",
          "cyan", "magenta", "gray60"]

def recsquare(l, f, colors):
    if not colors: return
    shapesize(l/20)
    color(colors[0])
    stamp()
    forward(l/2.0)
    left(90)
    forward(l/2)
    left(90)
    for _ in range(4):
        recsquare(l*f, f, colors[1:])
        forward(l)
        left(90)
    right(90)
    bk(l/2)
    right(90)
    back(l/2)
    if len(colors) == 5:
        update()

bgcolor("gray10")
shape("square")
hideturtle()

penup()
speed(0)
tracer(False)
ta = clock()
recsquare(256, 0.5, colors)
tb = clock()
print("{0:.2f}sec.".format(tb-ta))

mainloop()
