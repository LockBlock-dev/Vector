# gcc -nostdlib -static -march=rv32i -mabi=ilp32 hello.s

.global _start

.section .text
_start: addi  a0, x0, 1               # 1 = STDOUT
        # la    a1, helloworld          # load address of helloworld
        auipc a1, %hi(helloworld)     # load address of helloworld 1/2
        addi  a1, a1, %lo(helloworld) # load address of helloworld 2/2

        addi  a2, x0, 13              # length of the string
        addi  a7, x0, 64              # write syscall
        ecall                         # call linux

        addi  a0, x0, 0               # set 0 return code
        addi  a7, x0, 93              # exit syscall
        ecall                         # call linux

.data
helloworld: .ascii "Hello World!\n"
