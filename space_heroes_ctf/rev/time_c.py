
from pwn import *
import time
import ctypes

p = process("./Time")

lib = ctypes.CDLL('./libcalc.so')

#p = remote("spaceheroes-time.chals.io", 443, ssl=True, sni="spaceheroes-time.chals.io")

def calculate():
    # Define the return type of the calculate function
    lib.calculate.restype = ctypes.POINTER(ctypes.c_float)

    # Call the function
    result_ptr = lib.calculate()

    # Fetch results from the pointer to array
    result = [result_ptr[i] for i in range(12)]

    # Optionally, free the allocated memory if your C code does not handle it
    lib.free(result_ptr)

    return result

key = calculate()

print(key)

p.recvuntil(b"Find the time >>>")

for i in range(12):
    p.sendline(str(key[i])) # Send the modified array

p.interactive()