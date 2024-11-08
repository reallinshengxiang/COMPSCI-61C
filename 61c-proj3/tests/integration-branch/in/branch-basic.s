         addi s1, x0, 1
         addi s0, x0, -1
label5:  beq  x0, x0, label1
label6:  bltu x0, s0, label8
label4:  blt  s1, x0, label5
         beq  x0, x0, label6
label3:  beq  s1, s1, label4
label10: beq  s0, s0, end
label2:  blt  x0, s1, label3
label9:  blt  s0, s1, label10
label1:  beq  x0, x0, label2
label8:  bltu s1, s1, label9
         beq  s1, s1, label9
end:     addi s0, x0, 2
