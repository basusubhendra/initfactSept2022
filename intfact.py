#!/usr/bin/python3
import sys

def characterize(num):
    tuples = []
    idx = 0
    l = len(num)
    while True:
        tuples.append(num[idx % l] + num[(idx + 1) % l])
        idx = idx + 1
        if (idx + 1) % l == 0:
            break
    return tuples

def deriveRelation(pp):
    p0 = int(pp[0])
    p1 = int(pp[1])
    if p0 < p1:
        return 0
    elif p0 > p1:
        return 1
    elif p0 == p1:
        return 2
    elif p0 == 0:
        return 3
    elif p1 == 0:
        return 4
    return -1

if __name__ == "__main__":
    num = str(sys.argv[1])
    tuples = characterize(num)
    print(tuples)
    f=open("./pi.txt","r")
    g=open("./e.txt","r")
    f.close()
    g.close()
