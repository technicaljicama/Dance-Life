import random
import struct

level = 2
bpm = 139
i = 0
name = "assets/red_eyes.mp3"

def int2uint32(num):
    return struct.pack('I', num)
    # return num.to_bytes(4, 'little') 

# old c header gen
"""
print("#define BPM "+str(bpm))
print("const int bpm = "+str(bpm)+";")

print("int music[] = { ")

for num in range(bpm):
    if random.randint(0, level) == level-1 and not level == 1: 
        print("0,", end="")
    else:
        print(str(random.randint(1, 4))+",", end="")
    i += 1
    
print("};")
"""

f = open("assets/pack.ddp", "wb")
f.write(int2uint32(len(name)))
f.write(name.encode())
f.write(int2uint32(bpm))

for num in range(bpm):
    f.write(int2uint32(random.randint(0, 4)))
