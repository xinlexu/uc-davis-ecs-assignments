.type multiply, @function
.globl multiply

;// int multiply(int n1, int n2) {
multiply:
    pushl   %ebp
    movl    %esp, %ebp

    ;// if (n1 < 0) { return -1; }
    cmpl    $0x0, 0x8(%ebp)
    jl      RETURN_NEGATIVE_ONE

    ;// else if (n2 < 0) { return -1; }
    cmpl    $0x0, 0xc(%ebp)
    jl      RETURN_NEGATIVE_ONE

    ;// register int result = 0;
    movl    $0x0, %edx

    ;// for (unsigned sftamt = 0; sftamt < 31; sftamt++) {
    movb    $0x0, %cl
FOR_LOOP_START:
    cmpb    $0x1f, %cl
    ja      HANDLE_FOR_LOOP_UNSATISFIED

    ;// if ((n2 & 0x00000001) == 1) {
    movl    0xc(%ebp), %eax
    andl    $0x1, %eax
    cmpl    $0x1, %eax
    jne     JMP_TO_THE_SECOND_IF_BLOCK
    
    ;// result += n1 << sftamt;
    movl    0x8(%ebp), %eax
    shll    %cl, %eax
    addl    %edx, %eax
    movl    %eax, %edx

    ;// if (overflow) { return -1; }
    jo      RETURN_NEGATIVE_ONE

JMP_TO_THE_SECOND_IF_BLOCK:
    ;// if (result < 0) { return -1; }
    cmpl    $0x0, %edx
    jl     RETURN_NEGATIVE_ONE

    ;// n2 >>= 1; }
    movl    0xc(%ebp), %eax
    sarl    $0x1, %eax 
    movl    %eax, 0xc(%ebp)
    
    addb    $0x1, %cl    
    jmp     FOR_LOOP_START
    
HANDLE_FOR_LOOP_UNSATISFIED:
    ;// return result; }
    movl    %edx, %eax
    jmp     MULTIPLY_EXIT

RETURN_NEGATIVE_ONE:
    movl    $0xffffffff, %eax

MULTIPLY_EXIT:
    movl    %ebp, %esp
    popl    %ebp
    ret
