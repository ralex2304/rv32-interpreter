.global main

.section .rodata

string: .string "Hello riscv asm\n"
.equ string_size, . - string

.section .text
main:
    li a7, 64 # write syscall
    li a0, 1  # stdout
    la a1, string
    li a2, string_size - 1 # remove \0
    ecall

    ret

