addi t0, x0, 89
addi t1, x0, 17
mul t0, t0, t1

lui s0, 554580
addi s0, s0, 801
lui s1, 74565
addi s1, s1, 1656
mul t0, s0, s0
mul t1, s1, s1
mul t2, s0, s1
mul t2, s1, s0
