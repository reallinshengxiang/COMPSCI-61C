        addi s0 x0 3
        addi s1 x0 19
        addi ra x0 1000
        addi t0 x0 1
        addi t1 s0 0
        addi s0 x0 1
loop:   blt s1 t0 end
        mul s0 s0 t1
        addi t0 t0 1
        jal x0 loop
end:    add s0, x0, s0 # padding to fix venus oddity
