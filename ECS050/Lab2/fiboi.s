.type fibonacci, @function
.globl fibonacci

;// int fibonacci(int n) {
fibonacci:
    pushl   %ebp
    movl    %esp, %ebp
    subl    $0xc, %esp

    ;// int cache[3] = {0, 1, 1};
    movl    $0x0, -0xc(%ebp)
    movl    $0x1, -0x8(%ebp)
    movl    $0x1, -0x4(%ebp)

    ;// if (n == 0) { return cache[0]; }
    xorl    %eax, %eax
    cmpl    0x8(%ebp), %eax
    je      HANDLE_N_EQUALS_ZERO

    ;// else if (n == 1) { return cache[1]; }
    incl    %eax
    cmpl    0x8(%ebp), %eax
    je      HANDLE_N_EQUALS_ONE

    ;// for (unsigned i = 3; i <= *((unsigned*)&n); i++) {
    incl    %eax
    incl    %eax
    movl    %eax, %ecx
FOR_LOOP_START:
    cmpl    0x8(%ebp), %ecx
    ja      HANDLE_FOR_LOOP_UNSATISFIED

    ;// cache[0] = cache[1];
    movl    -0x8(%ebp), %eax
    movl    %eax, -0xc(%ebp)

    ;// cache[1] = cache[2];
    movl    -0x4(%ebp), %eax
    movl    %eax, -0x8(%ebp)

    ;// cache[2] = cache[0] + cache[1]; 
    addl    -0xc(%ebp), %eax

    ;// if (overflow) { return -1; } }
    jo      RETURN_NEGATIVE_ONE

    movl    %eax, -0x4(%ebp)
    incl    %ecx
    jmp     FOR_LOOP_START

RETURN_NEGATIVE_ONE:
    movl    $0xffffffff, %eax
    jmp     FIBONACCI_EXIT

HANDLE_N_EQUALS_ZERO:
    movl    -0xc(%ebp), %eax
    jmp     FIBONACCI_EXIT

HANDLE_N_EQUALS_ONE:
    movl    -0x8(%ebp), %eax
    jmp     FIBONACCI_EXIT

    ;// return cache[2]; }
HANDLE_FOR_LOOP_UNSATISFIED:
    movl    -0x4(%ebp), %eax

FIBONACCI_EXIT:
    addl    $0xc, %esp
    movl    %ebp, %esp
    popl    %ebp
    ret
