# gcc -nostdlib -static -march=rv32i -mabi=ilp32 hello.s

.global _start      # Provide program starting address to linker

.section .text
_start: addi  a0, x0, 1      # 1 = STDOUT
        # la    a1, helloworld # load address of helloworld
        auipc a1, %hi(helloworld) # load address of helloworld 1/2
        addi a1, a1, %lo(helloworld) # load address of helloworld 2/2

        addi  a2, x0, 13     # length of our string
        addi  a7, x0, 64     # linux write system call
        ecall                # Call linux to output the string

        addi    a0, x0, 0   # Use 0 return code
        addi    a7, x0, 93  # Service command code 93 terminates
        ecall               # Call linux to terminate the program

.data
helloworld:      .ascii "Hello World!\n"
