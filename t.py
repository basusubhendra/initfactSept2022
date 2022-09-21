#!/usr/bin/python3
f=open("./e.txt","r")
c=f.read(2)
g=open("./_e.txt","w")
g.write("30")
c=str(f.read(98))
g.write(c)
while True:
    c=str(f.read(100))
    if c == "" or len(c) == 0 or not c:
        break
    g.write(c)
g.close()
f.close()


