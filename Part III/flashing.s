ldr r6,=0x20200000
add r0,r6,#0x00000004
add r1,r6,#0x00000028
add r2,r6,#0x0000001c

ldr r7,=0x00040000
ldr r9,=0x00010000

str r7,[r0]

loop:
mov r5,#0x00400000

on:
cmp r5,#0x00200000
beq off 
mov r4,r5
sub r5,r4,#1
str r9,[r2]
b on

off:
cmp r5,#0
beq loop
mov r4,r5
sub r5,r4,#1
str r9,[r1]
b off

andeq r0,r0,r0