import random

level = 2
bpm = 139
i = 0
name = "tooslow_"

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
