        addi s0 x0 46 #n =46
        addi t0 x0 1 #i =1
        addi t1 x0 1 #i =1
        addi ra x0 1000


        beq s0, x0, Ret0
        addi s1, x0, 1
        beq s0, s1, Ret1
        addi s0, s0, -2
Loop:   beq s0, x0, RetF
        add s1, t0, t1
        addi t1, t0, 0
        addi t0, s1, 0
        addi s0, s0, -1
        jal x0, Loop
Ret0:   addi a0, x0, 0
        jal x0, Done
Ret1:   addi a0, x0, 1
        jal x0, Done
RetF:   add a0, x0, s1

Done:
