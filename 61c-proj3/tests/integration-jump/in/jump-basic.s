        addi s0, x0, 1  # 1, addr 0x00
        addi s1, x0, 2  # 2,  addr 0x04
        jal  x0, label1 # 3,  addr 0x08
        addi s1, s0, -1 # x,  addr 0x0C
        addi s1, s0, -2 # x,  addr 0x10
label1: addi s1, x0, 0  # 4,  addr 0x14
        addi s0, s1, 0  # 5,  addr 0x18
        jal  ra, label2 # 6,  addr 0x1C, ra --> 0x20
        addi s0, s0, 5  # 11,  addr 0x20
        addi s1, s1, 6  # 12,  addr 0x24
        jal  x0, label3 # 13,  addr 0x28
label2: addi s1, s1, 0  # 7,  addr 0x2C
        addi s0, s0, 0  # 8,  addr 0x30
        addi ra, ra, -1 # 9,  edge testing setup
        jalr x0, ra, 1  # 10, addr 0x38 (edge testing)
label3:
