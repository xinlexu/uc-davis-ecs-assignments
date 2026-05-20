.type fibonacci, @function
.globl fibonacci

;// int fibonacci(int n) {
fibonacci:
    pushl   %ebp
    movl    %esp, %ebp
    subl    $0x4, %esp

    ;// if (n < 0) { return -1; }
    xorl    %eax, %eax
    cmpl    %eax, 0x8(%ebp)
    jl      RETURN_NEGATIVE_ONE

    ;// else if (n == 0) { return 0; }
    je      FIBONACCI_EXIT

    ;// else if (n == 1) { return 1; }
    incl    %eax
    cmpl    %eax, 0x8(%ebp)
    je      FIBONACCI_EXIT

    ;// else if (n == 2) { return 1; }
    incl    %eax
    cmpl    %eax, 0x8(%ebp)
    decl    %eax
    je      FIBONACCI_EXIT

    ;// else { int result = fibor(n - 1);
    movl    0x8(%ebp), %eax
    subl    $0x1, %eax
    pushl   %eax
    call    fibonacci
    addl    $0x4, %esp
    movl    %eax, -0x4(%ebp)
    
    ;// if (result == -1) { return -1; }
    incl    %eax 
    jz      RETURN_NEGATIVE_ONE

    ;// result += fibonacci(n - 2);
    movl    0x8(%ebp), %eax
    subl    $0x2, %eax 
    pushl   %eax 
    call    fibonacci
    addl    $0x4, %esp
    addl    -0x4(%ebp), %eax

    ;// if (overflow) { return -1; } 
    jo      RETURN_NEGATIVE_ONE

    ;// return result; } }
    jmp     FIBONACCI_EXIT

RETURN_NEGATIVE_ONE:
    movl    $0xffffffff, %eax
    jmp     FIBONACCI_EXIT

FIBONACCI_EXIT:
    addl    $0x4, %esp
    movl    %ebp, %esp
    popl    %ebp
    ret
