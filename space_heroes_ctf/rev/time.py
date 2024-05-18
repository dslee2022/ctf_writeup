
from pwn import *
import time

#p = process("./Time")
p = remote("spaceheroes-time.chals.io", 443, ssl=True, sni="spaceheroes-time.chals.io")

def calculate(a1):
    # Get the current time
    current_time = time.localtime(time.time())

    # Extract minutes and hours, then calculate v2
    minutes = current_time.tm_min
    hours = current_time.tm_hour
    v2 = 60 * hours + minutes

    # Modify the elements of the array a1
    for i in range(12):
        a1[i] = (v2 % 12) + 2 * a1[i]



key = [
    1.2,
    0.80000001,
    2.5,
    3.0,
    1.5,
    2.0,
    0.69999999,
    1.8,
    2.2,
    1.0,
    0.5,
    2.3]

calculate(key)

print(key)

p.recvuntil(b"Find the time >>>")

for i in range(12):
    p.sendline(str(key[i])) # Send the modified array

p.interactive()