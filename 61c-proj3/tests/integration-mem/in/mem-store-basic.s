addi s0, x0, 200
lui  s1, 86627   # 0x15263748
addi s1, s1, 1864

sw   s1, 80(s0)
lw   a0, 80(s0)

sh   s1, 80(s0)
lw   a0, 80(s0)
sh   s1, 82(s0)
lw   a0, 80(s0)

sb   s1, 80(s0)
lw   a0, 80(s0)
sb   s1, 81(s0)
lw   a0, 80(s0)
sb   s1, 82(s0)
lw   a0, 80(s0)
sb   s1, 83(s0)
lw   a0, 80(s0)

sw   s1, -204(s0)
lw   a0, -204(s0)

sh   s1, -204(s0)
lw   a0, -204(s0)
sh   s1, -202(s0)
lw   a0, -204(s0)

sb   s1, -204(s0)
lw   a0, -204(s0)
sb   s1, -203(s0)
lw   a0, -204(s0)
sb   s1, -202(s0)
lw   a0, -204(s0)
sb   s1, -201(s0)
lw   a0, -204(s0)
