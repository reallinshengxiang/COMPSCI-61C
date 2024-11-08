addi s0, x0, 1919
lui  s0, 703710

lui  s0, 0
lui  s0, 1
lui  s0, 1048574
lui  s0, 1048575

addi t0, x0, 2047
addi t1, x0, -2048
addi t0, t0, 1919

lui s0, 699051     # 0xAAAAAAAA (yBLdQ1a4-JI)
addi s0, s0, -1366

andi t0, s0, 2047
andi t0, s0, -2048
ori  t0, s0, 2047
ori  t0, s0, -2048
xori t0, s0, 2047
xori t0, s0, -2048

slli t0, s0, 30
slli t0, s0, 31
srai t0, s0, 30
srai t0, s0, 31
srli t0, s0, 30
srli t0, s0, 31

slti t0, s0, 2047
slti t0, s0, -2048