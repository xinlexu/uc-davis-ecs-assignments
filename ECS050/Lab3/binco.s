# -------------------- data structures --------------------
/*
typedef struct CMD_ARG
{
    unsigned mask;
    unsigned arg1;
    unsigned arg2;
    OPTION option;
    OPERATOR operator;
} CMD_ARG;

typedef struct COMPUTE_RESULT
{
    unsigned result;
    unsigned mask;
    FLAG flag;
} COMPUTE_RESULT;
 */

# -------------------- pseudo codes --------------------
.equ NULL,          0
.equ TRUE,          1
.equ FALSE,         0
.equ EXIT_SUCCESS,  0
.equ EXIT_FAILURE,  1
.equ STDOUT,        1
.equ STDERR,        2
.equ SYS_exit,      1
.equ SYS_write,     4

# enum OPTION
.equ NONE_OPTION,       0x01
.equ OPT_PRINT_HEX,     0x02
.equ OPT_PRINT_DEC,     0x04 
.equ OPT_USE_SIGNED,    0x08
.equ OPT_SHOW_HELP,     0x10

# enum OPERATOR
.equ NONE_OPERATOR,         1
.equ OP_AND,                2
.equ OP_DIFF,               3
.equ OP_IOR,                4
.equ OP_LEFT_SHIFT,         5
.equ OP_MUL,                6
.equ OP_NEG,                7
.equ OP_ONES_COMPL,         8
.equ OP_RIGHT_SHIFT_ARITH,  9
.equ OP_SUM,                10
.equ OP_TWOS_COMPL,         11
.equ OP_XOR,                12

# enum OPTYPE
.equ NONE_OPTYPE,       0x00
.equ OPTYPE_UNARY,      0x01
.equ OPTYPE_PAIRED,     0x02
.equ OPTYPE_ARIRHMETIC, 0x04
.equ OPTYPE_LOGICAL,    0x08

# enum FLAG
.equ FLAG_OVERFLOW,     0x01
.equ FLAG_CARRY,        0x02
.equ FLAG_SIGNED,       0x04




.text 

.globl main 
# int main(int argc, char *argv[]);
main:
    pushl   %ebp
    movl    %esp, %ebp
    ;// CMD_ARG cmd_arg;
    ;// COMPUTE_RESULT compute_result;
    subl    $0x20, %esp     # -0xc(%ebp):  compute_result
                            # -0x20(%ebp): cmd_arg 

    ;// parser(argc, argv, &cmd_arg);
    pushl   %edx
    pushl   %ecx 
    leal    -0x20(%ebp), %eax 
    pushl   %eax 
    movl    0xc(%ebp), %eax 
    pushl   %eax 
    movl    0x8(%ebp), %eax 
    pushl   %eax 
    call    parser
    addl    $0xc, %esp
    popl    %ecx 
    popl    %edx

    ;// calculate(&cmd_arg, &compute_result);
    pushl   %edx 
    pushl   %ecx 
    leal    -0xc(%ebp), %eax 
    pushl   %eax 
    leal    -0x20(%ebp), %eax 
    pushl   %eax 
    call    calculate
    addl    $0x8, %esp
    popl    %ecx 
    popl    %edx

    ;// show(&cmd_arg, &compute_result);
    pushl   %edx 
    pushl   %ecx 
    leal    -0xc(%ebp), %eax 
    pushl   %eax 
    leal    -0x20(%ebp), %eax 
    pushl   %eax 
    call    show
    addl    $0x8, %esp
    popl    %ecx 
    popl    %edx

    ;// return 0;
    xorl    %eax, %eax
    addl    $0x20, %esp
    popl    %ebp
    ret 



.globl binco_write
# void binco_write(int fd, const void *buf, size_t count);
binco_write:
    pushl   %ebp 
    movl    %esp, %ebp 
    pushl   %ebx 
    movl    0x10(%ebp), %edx
    movl    0xc(%ebp), %ecx
    movl    0x8(%ebp), %ebx
    movl    $SYS_write, %eax
    int     $0x80 
    popl    %ebx 
    popl    %ebp
    ret 



.globl binco_exit 
# void binco_exit(int status);
binco_exit:
    pushl   %ebp 
    movl    %esp, %ebp 
    pushl   %ebx 
    movl    0x8(%ebp), %edx 
    movl    $SYS_exit, %eax 
    movl    %edx, %ebx 
    int     $0x80 
    popl    %ebx
    popl    %ebp
    ret 



.globl binco_strlen
# size_t binco_strlen(const char *s);
binco_strlen:
    pushl   %ebp 
    movl    %esp, %ebp
    movl    0x8(%ebp), %edx     # %edx: *s
    ;// size_t len = 0;
    xorl    %eax, %eax 
    ;// while (*(s + len) != '\0') { len++; }
binco_strlen_loop:
    cmpb    $0, (%edx, %eax)
    je      binco_strlen_loop_end
    incl    %eax
    jmp     binco_strlen_loop
binco_strlen_loop_end:
    ;// return len;
    popl    %ebp
    ret 



.globl binco_memset
# void binco_memset(void *s, int c, size_t n);
binco_memset:
    pushl   %ebp
    movl    %esp, %ebp
    movl    0x8(%ebp), %edx     # %edx: s
    movl    0xc(%ebp), %eax     # %al: c
    ;// for (size_t i = 0; i < n; i++)
    xorl    %ecx, %ecx 
binco_memset_loop:
    cmpl    0x10(%ebp), %ecx
    jae     binco_memset_loop_end
    ;// *((char*)s + i) = (char)c;
    movb    %al, (%edx, %ecx)
    incl    %ecx
    jmp     binco_memset_loop
binco_memset_loop_end:
    popl    %ebp
    ret



.globl fatal 
# void fatal(const char *str);
fatal:
    pushl   %ebp 
    movl    %esp, %ebp
    movl    0x8(%ebp), %edx     # %edx: s
    ;// binco_write(STDERR, str, binco_strlen(str));
    pushl   %edx 
    pushl   %ecx 
    pushl   %edx 
    call    binco_strlen
    addl    $0x4, %esp
    popl    %ecx 
    popl    %edx

    pushl   %edx 
    pushl   %ecx 
    pushl   %eax
    pushl   %edx 
    pushl   $STDERR
    call    binco_write
    addl    $0xc, %esp
    popl    %ecx
    popl    %edx 

    ;// binco_write(STDERR, fatal_string_data_linefeed, binco_strlen(fatal_string_data_linefeed));
    pushl   %edx 
    pushl   %ecx 
    pushl   $fatal_string_data_linefeed
    call    binco_strlen
    addl    $0x4, %esp
    popl    %ecx 
    popl    %edx

    pushl   %edx 
    pushl   %ecx 
    pushl   %eax
    pushl   $fatal_string_data_linefeed
    pushl   $STDERR
    call    binco_write
    addl    $0xc, %esp
    popl    %ecx
    popl    %edx 

    ;// binco_exit(EXIT_FAILURE);
    pushl   %edx 
    pushl   %ecx 
    pushl   $EXIT_FAILURE
    call    binco_exit
    addl    $0x4, %esp
    popl    %ecx 
    popl    %edx 

    popl    %ebp
    ret 
fatal_string_data_linefeed:
    .string "\n"



.globl binco_max
# unsigned binco_max(unsigned a, unsigned b);
binco_max:
    pushl   %ebp
    movl    %esp, %ebp
    ;// return a > b ? a : b;
    movl    0x8(%ebp), %eax
    cmpl    %eax, 0xc(%ebp)
    jae     binco_max_ret_b
    jmp     binco_max_ret
binco_max_ret_b:
    movl    0xc(%ebp), %eax
binco_max_ret:
    popl    %ebp
    ret



.globl len2mask
# unsigned len2mask(unsigned len);
len2mask:
    pushl   %ebp
    movl    %esp, %ebp
    movl    0x8(%ebp), %ecx     # %ecx: len

    ;// for (result = 0; (int)len > 0; len--) 
    xorl    %eax, %eax          # %eax: result
len2mask_loop:
    cmpl    $0, %ecx 
    jle     len2mask_loop_end
    ;// { result <<= 1; result++; }
    sall    $1, %eax
    incl    %eax
    decl    %ecx 
    jmp     len2mask_loop
len2mask_loop_end:
    ;// return result;
    popl    %ebp
    ret


.globl mask2len
# unsigned mask2len(unsigned mask);
mask2len:
    pushl   %ebp
    movl    %esp, %ebp
    movl    0x8(%ebp), %ecx     # %ecx: mask
    ;// for (result = 0; mask > 0 && result < 32; result++)
    xorl    %eax, %eax          # %eax: result
mask2len_loop:
    cmpl    $0, %ecx
    jbe     mask2len_loop_end
    cmpl    $32, %eax 
    jae     mask2len_loop_end
    ;// mask >>= 1;
    shrl    $1, %ecx
    incl    %eax 
    jmp     mask2len_loop
mask2len_loop_end:
    ;// return result;
    popl    %ebp
    ret




calculate_jump_table: # 25 elements
    .long calculate_case_default
    .long calculate_case_NONE_OPERATOR
    .long calculate_case_OP_AND
    .long calculate_case_OP_DIFF
    .long calculate_case_OP_IOR
    .long calculate_case_OP_LEFT_SHIFT
    .long calculate_case_OP_MUL
    .long calculate_case_OP_NEG
    .long calculate_case_OP_ONES_COMPL
    .long calculate_case_OP_RIGHT_SHIFT_ARITH
    .long calculate_case_OP_SUM
    .long calculate_case_OP_TWOS_COMPL
    .long calculate_case_OP_XOR

get_optype_jump_table: # 25 elements
    .long get_optype_case_A
    .long get_optype_case_B
    .long get_optype_case_C
    .long get_optype_case_D
    .long get_optype_case_E
    .long get_optype_case_F
    .long get_optype_case_G
    .long get_optype_case_H
    .long get_optype_case_I
    .long get_optype_case_J
    .long get_optype_case_K
    .long get_optype_case_L
    .long get_optype_case_M
    .long get_optype_case_N
    .long get_optype_case_O
    .long get_optype_case_P
    .long get_optype_case_Q
    .long get_optype_case_R
    .long get_optype_case_S
    .long get_optype_case_T
    .long get_optype_case_U
    .long get_optype_case_V
    .long get_optype_case_W
    .long get_optype_case_X
    .long get_optype_case_default
    
get_operator_jump_table:
    .long get_operator_case_A
    .long get_operator_case_B
    .long get_operator_case_C
    .long get_operator_case_D
    .long get_operator_case_E
    .long get_operator_case_F
    .long get_operator_case_G
    .long get_operator_case_H
    .long get_operator_case_I
    .long get_operator_case_J
    .long get_operator_case_K
    .long get_operator_case_L
    .long get_operator_case_M
    .long get_operator_case_N
    .long get_operator_case_O
    .long get_operator_case_P
    .long get_operator_case_Q
    .long get_operator_case_R
    .long get_operator_case_S
    .long get_operator_case_T
    .long get_operator_case_U
    .long get_operator_case_V
    .long get_operator_case_W
    .long get_operator_case_X
    .long get_operator_case_default




.globl calculate
# void calculate(CMD_ARG *cmd_arg, COMPUTE_RESULT *compute_result);
calculate:
    pushl   %ebp
    movl    %esp, %ebp
    movl    0xc(%ebp), %edx     # %edx: compute_result
    movl    0x8(%ebp), %ecx     # %ecx: cmd_arg

    ;// binco_memset(compute_result, 0, sizeof(COMPUTE_RESULT));
    pushl   %edx 
    pushl   %ecx 
    pushl   $0xc
    pushl   $0
    pushl   0xc(%ebp)
    call    binco_memset
    addl    $0xc, %esp 
    popl    %ecx 
    popl    %edx

    movl    0x8(%ebp), %eax 
    movl    0x10(%eax), %eax        # %eax: cmd_arg->opearetor
    cmpl    $0x19, %eax        
    jae     calculate_case_default  # switch table out of bound
    ;// switch (cmd_arg->operator)
    movl    calculate_jump_table(, %eax, 4), %eax
    pushl   %edx 
    pushl   %ecx 
    pushl   %edx 
    pushl   %ecx
    jmp     *%eax
calculate_ret:
    addl    $0x8, %esp
    popl    %ecx 
    popl    %edx 

    popl    %ebp
    ret

calculate_case_default:
    pushl   $calculate_string_data
    call    fatal
    addl    $0x4, %esp
    jmp     calculate_ret
calculate_case_NONE_OPERATOR: 
    call    calculate_none_operator
    jmp     calculate_ret
calculate_case_OP_AND: 
    call    And
    jmp     calculate_ret
calculate_case_OP_DIFF: 
    call    Diff
    jmp     calculate_ret
calculate_case_OP_IOR: 
    call    InclusiveOR
    jmp     calculate_ret
calculate_case_OP_LEFT_SHIFT: 
    call    LeftShift
    jmp     calculate_ret
calculate_case_OP_MUL: 
    call    Multiply
    jmp     calculate_ret
calculate_case_OP_NEG: 
    call    Negate
    jmp     calculate_ret
calculate_case_OP_ONES_COMPL: 
    call    OnesComplement
    jmp     calculate_ret
calculate_case_OP_RIGHT_SHIFT_ARITH: 
    call    RightShiftArithmetic
    jmp     calculate_ret
calculate_case_OP_SUM: 
    call    Sum
    jmp     calculate_ret
calculate_case_OP_TWOS_COMPL: 
    call    TwosComplement
    jmp     calculate_ret
calculate_case_OP_XOR: 
    call    XOR
    jmp     calculate_ret

calculate_string_data:
    .string "Error: bad operation reached in calculator()"



.globl And
# void And(CMD_ARG *cmd_arg, COMPUTE_RESULT *compute_result);
And:
    pushl   %ebp 
    movl    %esp, %ebp 
    movl    0xc(%ebp), %edx     # %edx: compute_result 
    movl    0x8(%ebp), %ecx     # %ecx: cmd_arg
    
    ;// compute_result->result = cmd_arg->arg1 & cmd_arg->arg2;
    movl    0x4(%ecx), %eax
    andl    0x8(%ecx), %eax 
    movl    %eax, (%edx)
    
    ;// compute_result->mask = cmd_arg->mask;
    movl    (%ecx), %eax 
    movl    %eax, 0x4(%edx)

    popl    %ebp 
    ret 



.globl Diff
# void Diff(CMD_ARG *cmd_arg, COMPUTE_RESULT *compute_result);
Diff:
    pushl   %ebp 
    movl    %esp, %ebp
    movl    0x8(%ebp), %ecx     # %ecx: cmd_arg
    movl    0xc(%ebp), %edx     # %edx: compute_result

    ;// if (cmd_arg->option & OPT_USE_SIGNED)
    testl   $OPT_USE_SIGNED, 0xc(%ecx)
    jz      Diff_else_block    

    ;// if (cmd_arg->arg1 >> (mask2len(cmd_arg->mask) - 1) == 1)
    pushl   %edx
    pushl   %ecx 
    pushl   (%ecx)
    call    mask2len
    addl    $0x4, %esp 
    popl    %ecx 
    popl    %edx 
    decl    %eax 

    pushl   %edx
    movl    0x4(%ecx), %edx
    pushl   %ecx 
    movl    %eax, %ecx  
    shrl    %cl, %edx
    popl    %ecx
    movl    %edx, 0x4(%ecx)
    popl    %edx 
    
    cmpl    $1, 0x4(%ecx)
    jne     Diff_if_inner_second_if_block
    ;// cmd_arg->arg1 += 0xffffffff ^ cmd_arg->mask;
    movl    (%ecx), %eax 
    xorl    $0xffffffff, %eax 
    addl    %eax, 0x4(%ecx)

Diff_if_inner_second_if_block:
    ;// if (cmd_arg->arg2 >> (mask2len(cmd_arg->mask) - 1) == 1)
    pushl   %edx
    pushl   %ecx 
    pushl   (%ecx)
    call    mask2len
    addl    $0x4, %esp 
    popl    %ecx 
    popl    %edx 
    decl    %eax 

    pushl   %edx
    movl    0x8(%ecx), %edx
    pushl   %ecx 
    movl    %eax, %ecx  
    shrl    %cl, %edx
    popl    %ecx
    movl    %edx, 0x8(%ecx)
    popl    %edx 
    
    cmpl    $1, 0x8(%ecx)
    jne     Diff_if_inner_second_if_block_end
    ;// cmd_arg->arg2 += 0xffffffff ^ cmd_arg->mask;
    movl    (%ecx), %eax 
    xorl    $0xffffffff, %eax 
    addl    %eax, 0x8(%ecx)

Diff_if_inner_second_if_block_end:
    ;// cmd_arg->arg2 = (cmd_arg->arg2 ^ 0xffffffff) + 1;
    movl    0x8(%ecx), %eax 
    xorl    $0xffffffff, %eax 
    incl    %eax 
    movl    %eax, 0x8(%ecx)
    
    ;// Sum(cmd_arg, compute_result);
    pushl   %edx 
    pushl   %ecx 
    pushl   %edx 
    pushl   %ecx 
    call    Sum
    addl    $0x8, %esp 
    popl    %ecx
    popl    %edx 

Diff_else_block:
    ;// if (cmd_arg->arg1 < cmd_arg->arg2)
    movl    0x4(%ecx), %eax 
    cmpl    %eax, 0x8(%ecx)
    jbe     Diff_else_block_if_end
    pushl   Diff_string_data_unsigned_lesser
    call    fatal 
    addl    $0x4, %esp 
Diff_else_block_if_end:

    ;// compute_result->result = cmd_arg->arg1 - cmd_arg->arg2;
    movl    0x4(%ecx), %eax 
    subl    0x8(%ecx), %eax 
    movl    %eax, (%edx)

    ;// compute_result->mask = cmd_arg->mask;
    movl    (%ecx), %eax 
    movl    %eax, 0x4(%edx)

    popl    %ebp
    ret 

Diff_string_data_unsigned_lesser:
    .string "Error: arg1 lesser than arg2 when opearting unsigned different."




.globl InclusiveOR
# void InclusiveOR(CMD_ARG *cmd_arg, COMPUTE_RESULT *compute_result);
InclusiveOR:
    pushl   %ebp 
    movl    %esp, %ebp 
    movl    0xc(%ebp), %edx     # %edx: compute_result 
    movl    0x8(%ebp), %ecx     # %ecx: cmd_arg
    
    ;// compute_result->result = cmd_arg->arg1 | cmd_arg->arg2;
    movl    0x4(%ecx), %eax
    orl     0x8(%ecx), %eax 
    movl    %eax, (%edx)
    
    ;// compute_result->mask = cmd_arg->mask;
    movl    (%ecx), %eax 
    movl    %eax, 0x4(%edx)

    popl    %ebp 
    ret 



.globl LeftShift
# void LeftShift(CMD_ARG *cmd_arg, COMPUTE_RESULT *compute_result);
LeftShift:
    pushl   %ebp 
    movl    %esp, %ebp 
    movl    0xc(%ebp), %edx     # %edx: compute_result 
    movl    0x8(%ebp), %ecx     # %ecx: cmd_arg

    ;// if (cmd_arg->arg2 >= 32)
    cmpl    $0x20, 0x8(%ecx)
    jb      LeftShift_else_block
    ;// compute_result->result = 0; compute_result->mask = len2mask(1);
    movl    $0, (%edx)
    movl    $0x00000001, 0x4(%edx)
    jmp     LeftShift_ret

LeftShift_else_block:
    ;// compute_result->result = cmd_arg->arg1 << cmd_arg->arg2;
    pushl   %ecx 
    movl    0x4(%ecx), %eax 
    movb    0x8(%ecx), %cl
    sall    %cl, %eax
    movl    %eax, (%edx)
    popl    %ecx
    ;// compute_result->mask = len2mask(mask2len(cmd_arg->mask) + cmd_arg->arg2);
    movl    (%ecx), %eax 
    pushl   %edx 
    pushl   %ecx 
    pushl   %eax 
    call    mask2len
    addl    $0x4, %esp 
    popl    %ecx 
    popl    %edx 
    addl    0x8(%ecx), %eax 

    pushl   %edx 
    pushl   %ecx 
    pushl   %eax 
    call    len2mask
    addl    $0x4, %esp 
    popl    %ecx 
    popl    %edx 
    movl    %eax, 0x4(%edx)

    ;// if (mask2len(cmd_arg->mask) + cmd_arg->arg2 >= 32)
    pushl   %edx 
    pushl   %ecx 
    pushl   (%ecx)
    call    mask2len
    addl    $0x4, %esp
    popl    %ecx
    popl    %edx
    addl    0x8(%ecx), %eax 
    cmpl    $0x20, %eax 
    jb      LeftShift_else_block_inner_second_if_block
    ;// compute_result->mask = len2mask(32);
    movl    $0xffffffff, 0x4(%edx)

LeftShift_else_block_inner_second_if_block:
    ;// if (cmd_arg->option & OPT_USE_SIGNED)
    testl   $OPT_USE_SIGNED, 0xc(%ecx)
    jz      LeftShift_ret
    ;// compute_result->flag |= FLAG_SIGNED;
    orl     $FLAG_SIGNED, 0x8(%edx)
LeftShift_ret:
    popl    %ebp 
    ret 




.globl Multiply
# void Multiply(CMD_ARG *cmd_arg, COMPUTE_RESULT *compute_result);
Multiply:
    pushl   %ebp 
    movl    %esp, %ebp 
    movl    0xc(%ebp), %edx     # %edx: compute_result 
    movl    0x8(%ebp), %ecx     # %ecx: cmd_arg

    ;// compute_result->result = cmd_arg->arg1 * cmd_arg->arg2;
    pushl   %edx 
    pushl   %ecx
    movl    0x4(%ecx), %eax
    movl    0x8(%ecx), %ecx
    mull    %ecx 
    popl    %ecx 
    popl    %edx 
    movl    %eax, (%edx)

    ;// unsigned len = 0; while (compute_result->result >> len != 0) { len++; }
    pushl   %ecx
    xorl    %ecx, %ecx 
Multiply_loop:
    movl    (%edx), %eax 
    shrl    %cl, %eax 
    cmpl    $0, %eax 
    je      Multiply_loop_end
    incl    %ecx 
    jmp     Multiply_loop
Multiply_loop_end:
    popl    %ecx

    ;// compute_result->mask = len2mask(len);
    pushl   %edx 
    pushl   %ecx 
    pushl   %ecx 
    call    len2mask
    addl    $0x4, %esp 
    popl    %ecx 
    popl    %edx 
    movl    %eax, 0x4(%edx)

    popl    %ebp 
    ret 



.globl Negate
# void Negate(CMD_ARG *cmd_arg, COMPUTE_RESULT *compute_result);
Negate:
    pushl   %ebp 
    movl    %esp, %ebp 
    movl    0xc(%ebp), %edx     # %edx: compute_result 
    movl    0x8(%ebp), %ecx     # %ecx: cmd_arg
    
    ;// compute_result->result = -cmd_arg->arg1;
    movl    0x4(%ecx), %eax
    negl    %eax 
    movl    %eax, (%edx)
    
    ;// compute_result->mask = cmd_arg->mask;
    movl    (%ecx), %eax 
    movl    %eax, 0x4(%edx)

    popl    %ebp 
    ret 



.globl OnesComplement
# void OnesComplement(CMD_ARG *cmd_arg, COMPUTE_RESULT *compute_result);
OnesComplement:
    pushl   %ebp 
    movl    %esp, %ebp 
    movl    0xc(%ebp), %edx     # %edx: compute_result 
    movl    0x8(%ebp), %ecx     # %ecx: cmd_arg
    
    ;// compute_result->result = cmd_arg->arg1 ^ 0xffffffff;
    movl    0x4(%ecx), %eax
    xorl    $0xffffffff, %eax 
    movl    %eax, (%edx)
    
    ;// compute_result->mask = cmd_arg->mask;
    movl    (%ecx), %eax 
    movl    %eax, 0x4(%edx)

    popl    %ebp 
    ret 



.globl RightShiftArithmetic
# void RightShiftArithmetic(CMD_ARG *cmd_arg, COMPUTE_RESULT *compute_result);
RightShiftArithmetic:
    pushl   %ebp 
    movl    %esp, %ebp 
    movl    0xc(%ebp), %edx     # %edx: compute_result 
    movl    0x8(%ebp), %ecx     # %ecx: cmd_arg

    ;// if (cmd_arg->arg2 >= mask2len(cmd_arg->mask))
    pushl   %edx 
    pushl   %ecx 
    pushl   (%ecx)
    call    mask2len
    addl    $0x4, %esp
    popl    %ecx 
    popl    %edx 
    cmpl    %eax, 0x8(%ecx)
    jb      RightShiftArithmetic_else_block
    ;// compute_result->result = 0; compute_result->mask = len2mask(1);
    movl    $0, (%edx)
    movl    $0x00000001, 0x4(%edx)
    jmp     RightShiftArithmetic_ret
RightShiftArithmetic_else_block:
    ;// compute_result->result = cmd_arg->arg1 >> cmd_arg->arg2;
    pushl   %ecx
    movl    0x4(%ecx), %eax 
    movl    0x8(%ecx), %ecx
    sarl    %cl, %eax
    movl    %eax, (%edx)
    popl    %ecx
    ;// compute_result->mask = len2mask(mask2len(cmd_arg->mask) - cmd_arg->arg2);
    pushl   %edx 
    pushl   %ecx 
    pushl   (%ecx)
    call    mask2len
    addl    $0x4, %esp
    popl    %ecx 
    popl    %edx 
    subl    0x8(%ecx), %eax 

    pushl   %edx 
    pushl   %ecx 
    pushl   %eax 
    call    len2mask
    addl    $0x4, %esp 
    popl    %ecx 
    popl    %edx 
    movl    %eax, 0x4(%edx)

RightShiftArithmetic_ret:
    popl    %ebp 
    ret 


.globl Sum
# void Sum(CMD_ARG *cmd_arg, COMPUTE_RESULT *compute_result);
Sum:
    pushl   %ebp
    movl    %esp, %ebp 
    subl    $0x10, %esp         # -0x4(%ebp): result
                                # -0x8(%ebp): msb_cout
                                # -0xc(%ebp): smsb_cout
                                # -0x10(%ebp): result_len
    movl    0x8(%ebp), %ecx     # %ecx: cmd_arg
    movl    0xc(%ebp), %edx     # %edx: compute_result

    ;// unsigned result = cmd_arg->arg1 + cmd_arg->arg2;
    movl    0x4(%ecx), %eax 
    addl    0x8(%ecx), %eax 
    movl    %eax, -0x4(%ebp)

    ;// unsigned msb_cout = result >> mask2len(cmd_arg->mask);
    pushl   %edx 
    pushl   %ecx 
    pushl   (%ecx)
    call    mask2len
    addl    $0x4, %esp
    popl    %ecx
    popl    %edx 
    
    pushl   %ecx
    movl    %eax, %ecx  
    movl    -0x4(%ebp), %eax
    shrl    %cl, %eax 
    popl    %ecx 
    movl    %eax, -0x8(%ebp)

    ;// unsigned smsb_cout = ((cmd_arg->arg1 & (cmd_arg->mask >> 1)) + (cmd_arg->arg2 & (cmd_arg->mask >> 1))) >> (mask2len(cmd_arg->mask) - 1);
    pushl   %edi 
    pushl   %esi

    ;// %esi = (mask2len(cmd_arg->mask) - 1)
    pushl   %edx
    pushl   %ecx 
    pushl   (%ecx)
    call    mask2len
    addl    $0x4, %esp 
    popl    %ecx
    popl    %edx 
    decl    %eax 
    movl    %eax, %esi 
    ;// %edi = (cmd_arg->arg1 & (cmd_arg->mask >> 1))
    movl    (%ecx), %eax 
    shrl    $1, %eax 
    andl    0x4(%ecx), %eax 
    movl    %eax, %edi 
    ;// %edi += (cmd_arg->arg2 & (cmd_arg->mask >> 1))
    movl    (%ecx), %eax 
    shrl    $1, %eax 
    andl    0x8(%ecx), %eax 
    addl    %eax, %edi 
    ;// %edi >>= %esi
    pushl   %ecx 
    movl    %esi, %ecx 
    shrl    %cl, %edi
    popl    %ecx 
    ;// smb_cout = %edi
    movl    %edi, -0xc(%ebp)

    popl    %esi 
    popl    %edi 

    ;// if (cmd_arg->option & OPT_USE_SIGNED)
    testl   $OPT_USE_SIGNED, 0xc(%ecx)
    jz      Sum_else_block

    ;// if (msb_cout == 1) { compute_result->flag |= FLAG_CARRY; }
    cmpl    $1, -0x8(%ebp)
    jne     Sum_if_block_inner_second_if_block
    orl     $FLAG_CARRY, 0x8(%edx)

Sum_if_block_inner_second_if_block:
    ;// if ((msb_cout == 1) ^ (smsb_cout)) { compute_result->flag |= FLAG_OVERFLOW; }
    cmpl    $1, -0x8(%ebp)
    je      Sum_if_block_inner_second_if_block_msb_cout_equals_1_true
    jmp     Sum_if_block_inner_second_if_block_msb_cout_equals_1_false
Sum_if_block_inner_second_if_block_msb_cout_equals_1_true:
    movl    $1, %eax 
    xorl    -0xc(%ebp), %eax 
    cmpl    $0, %eax 
    je      Sum_if_block_inner_third_if_block
    ;// compute_result->flag |= FLAG_OVERFLOW;
    orl     $FLAG_OVERFLOW, 0x8(%edx)
    jmp     Sum_if_block_inner_third_if_block
Sum_if_block_inner_second_if_block_msb_cout_equals_1_false:
    movl    $0, %eax 
    xorl    -0xc(%ebp), %eax 
    cmpl    $0, %eax 
    je      Sum_if_block_inner_third_if_block
    ;// compute_result->flag |= FLAG_OVERFLOW;
    orl     $FLAG_OVERFLOW, 0x8(%edx)
    jmp     Sum_if_block_inner_third_if_block

Sum_if_block_inner_third_if_block:
    ;// if (msb_cout == 1) { compute_result->flag |= FLAG_SIGNED; }
    cmpl    $1, -0x8(%ebp)
    jne     Sum_else_block_end
    orl     $FLAG_SIGNED, 0x8(%edx)
    jmp     Sum_else_block_end

Sum_else_block:
    ;// if (msb_cout == 1) { compute_result->flag |= FLAG_CARRY; }
    cmpl    $1, -0x8(%ebp)
    jne     Sum_else_block_second_if_block
    orl     $FLAG_CARRY, 0x8(%edx)

Sum_else_block_second_if_block:
    ;// if (smsb_cout == 0 && msb_cout == 1) 
    cmpl    $0, -0xc(%ebp)
    jne     Sum_else_block_end
    cmpl    $1, -0x8(%ebp)
    jne     Sum_else_block_end
    ;// compute_result->flag |= FLAG_OVERFLOW;
    orl     $FLAG_OVERFLOW, 0x8(%edx)

Sum_else_block_end:
    ;// compute_result->result = result;
    movl    -0x4(%ebp), %eax 
    movl    %eax, (%edx)

    ;// for (result_len = 0; result_len < 32; result_len++)
    pushl   %ecx 
    xorl    %ecx, %ecx 
Sum_loop:
    cmpl    $0x20, %ecx 
    jae     Sum_loop_end

    ;// if (result >> result_len == 0) { break; }
    movl    -0x4(%ebp), %eax 
    shrl    %cl, %eax 
    cmpl    $0, %eax     
    jne     Sum_loop_continue
    jmp     Sum_loop_end
Sum_loop_continue:
    incl    %ecx 
    jmp     Sum_loop
Sum_loop_end:
    movl    %ecx, -0x10(%ebp)
    popl    %ecx 

    ;// if (result_len == 0) { result_len = 1; }
    cmpl    $0, -0x10(%ebp)
    jne     Sum_last_else_if_block
    movl    $1, -0x10(%ebp)
    jmp     Sum_last_else_if_block_end
Sum_last_else_if_block:
    ;// else if (result_len == 32)
    cmpl    $0x20, -0x10(%ebp)
    jne     Sum_last_else_if_block_end
    ;// compute_result->result = (0xffffffff ^ compute_result->result) + 1;
    movl    (%edx), %eax 
    xorl    $0xffffffff, %eax
    incl    %eax 
    movl    %eax, (%edx)
    ;// result_len = mask2len(cmd_arg->mask) + 1;
    pushl   %edx 
    pushl   %ecx 
    pushl   (%ecx)
    call    mask2len
    addl    $0x4, %esp 
    incl    %eax
    popl    %ecx 
    popl    %edx
    movl    %eax, -0x10(%ebp)
    ;// compute_result->flag |= FLAG_SIGNED;
    orl     $FLAG_SIGNED, 0x8(%edx)

Sum_last_else_if_block_end:
    ;// compute_result->mask = len2mask(result_len);
    pushl   %edx 
    pushl   %ecx 
    pushl   -0x10(%ebp)
    call    len2mask
    addl    $0x4, %esp 
    popl    %ecx 
    popl    %edx 
    movl    %eax, 0x4(%edx)

    addl    $0x10, %esp 
    popl    %ebp
    ret 




.globl TwosComplement
# void TwosComplement(CMD_ARG *cmd_arg, COMPUTE_RESULT *compute_result);
TwosComplement:
    pushl   %ebp 
    movl    %esp, %ebp 
    movl    0xc(%ebp), %edx     # %edx: compute_result 
    movl    0x8(%ebp), %ecx     # %ecx: cmd_arg
    
    ;// compute_result->result = (cmd_arg->arg1 ^ 0xffffffff) + 1;
    movl    0x4(%ecx), %eax
    xorl    $0xffffffff, %eax 
    incl    %eax
    movl    %eax, (%edx)
    
    ;// compute_result->mask = cmd_arg->mask;
    movl    (%ecx), %eax 
    movl    %eax, 0x4(%edx)

    popl    %ebp 
    ret 



.globl XOR
# void XOR(CMD_ARG *cmd_arg, COMPUTE_RESULT *compute_result);
XOR:
    pushl   %ebp 
    movl    %esp, %ebp 
    movl    0xc(%ebp), %edx     # %edx: compute_result 
    movl    0x8(%ebp), %ecx     # %ecx: cmd_arg
    
    ;// compute_result->result = cmd_arg->arg1 ^ cmd_arg->arg2;
    movl    0x4(%ecx), %eax
    xorl    0x8(%ecx), %eax 
    movl    %eax, (%edx)
    
    ;// compute_result->mask = cmd_arg->mask;
    movl    (%ecx), %eax 
    movl    %eax, 0x4(%edx)

    popl    %ebp 
    ret 



.globl calculate_none_operator
# void calculate_none_operator(CMD_ARG *cmd_arg, COMPUTE_RESULT *compute_result);
calculate_none_operator:
    pushl   %ebp 
    movl    %esp, %ebp 
    movl    0xc(%ebp), %edx     # %edx: compute_result 
    movl    0x8(%ebp), %ecx     # %ecx: cmd_arg
    
    ;// compute_result->result = cmd_arg->arg1;
    movl    0x4(%ecx), %eax
    movl    %eax, (%edx)
    
    ;// compute_result->mask = cmd_arg->mask;
    movl    (%ecx), %eax 
    movl    %eax, 0x4(%edx)

    popl    %ebp 
    ret 




# void parser(int argc, char *argv[], CMD_ARG *cmd_arg);




.globl is_help
# bool is_help(const char *str);
is_help:
    pushl   %ebp 
    movl    %esp, %ebp
    movl    0x8(%ebp), %edx 

    ;// if (*(str) == '-' && *(str + 1) == '?' && *(str + 2) == '\0')
    cmpb    $0x2d, (%edx)
    jne     is_help_ret_false
    cmpb    $0x3f, 0x1(%edx)
    jne     is_help_ret_false
    cmpb    $0, 0x2(%edx)
    jne     is_help_ret_false
    movl    $TRUE, %eax 
    jmp     is_help_ret
is_help_ret_false:
    movl    $FALSE, %eax 
is_help_ret:
    popl    %ebp
    ret 




.globl is_format
# bool is_format(const char *str);
is_format:
    pushl   %ebp 
    movl    %esp, %ebp
    movl    0x8(%ebp), %edx 

    ;// if (*(str) == '-' && (*(str + 1) == 'h' || *(str + 1) == 'd') && *(str + 2) == '\0')
    cmpb    $0x2d, (%edx)
    jne     is_format_ret_false
    cmpb    $0, 0x2(%edx)
    jne     is_format_ret_false
    cmpb    $0x68, 0x1(%edx)
    je      is_format_ret_true
    cmpb    $0x64, 0x1(%edx)
    je      is_format_ret_true
    jmp     is_format_ret_false
is_format_ret_true:
    movl    $TRUE, %eax 
    jmp     is_format_ret
is_format_ret_false:
    movl    $FALSE, %eax 
is_format_ret:
    popl    %ebp
    ret 




.globl is_signed
# bool is_signed(const char *str);
is_signed:
    pushl   %ebp 
    movl    %esp, %ebp
    movl    0x8(%ebp), %edx 

    ;// if (*(str) == '-' && *(str + 1) == 's' && *(str + 2) == '\0')
    cmpb    $0x2d, (%edx)
    jne     is_signed_ret_false
    cmpb    $0x73, 0x1(%edx)
    jne     is_signed_ret_false
    cmpb    $0, 0x2(%edx)
    jne     is_signed_ret_false
    movl    $TRUE, %eax 
    jmp     is_signed_ret
is_signed_ret_false:
    movl    $FALSE, %eax 
is_signed_ret:
    popl    %ebp
    ret 




.globl is_option
# bool is_option(const char *str);
is_option:
    pushl   %ebp 
    movl    %esp, %ebp

    ;// if (is_help(str) || is_format(str) || is_signed(str))
    pushl   0x8(%ebp)
    call    is_help
    addl    $0x4, %esp 
    cmpl    $0, %eax 
    jnz     is_option_ret_true
    pushl   0x8(%ebp)
    call    is_format
    addl    $0x4, %esp 
    cmpl    $0, %eax 
    jnz     is_option_ret_true
    pushl   0x8(%ebp)
    call    is_signed
    addl    $0x4, %esp 
    cmpl    $0, %eax 
    jnz     is_option_ret_true
    
    ;// else if (*str == '-')
    movl    0x8(%ebp), %eax 
    cmpl    $0x2d, (%eax)
    jnz     is_option_ret_false
    pushl   $is_option_string_data
    call    fatal 
    addl    $0x4, %esp 
    jmp     is_option_ret_false
is_option_ret_true:
    movl    $TRUE, %eax 
    jmp     is_option_ret 
is_option_ret_false:
    movl    $FALSE, %eax
is_option_ret:
    popl    %ebp
    ret 

is_option_string_data:
    .string "Error: Bad option.";




.globl get_option
# OPTION get_option(const char *str);
get_option:
    pushl   %ebp
    movl    %esp, %ebp
    movl    0x8(%ebp), %edx     # %edx: str

    ;// if (!is_option(str)) { return NONE_OPTION; }
    pushl   %edx 
    pushl   %ecx 
    pushl   %edx 
    call    is_option
    addl    $0x4, %esp 
    popl    %ecx 
    popl    %edx
    cmpl    $0, %eax 
    jnz     get_option_else_if_help
    jmp     get_option_else
get_option_else_if_help:
    cmpb    $0x3f, 0x1(, %edx, 1)
    jne     get_option_else_if_hex
    movl    $OPT_SHOW_HELP, %eax
    jmp     get_option_ret
get_option_else_if_hex:
    cmpb    $0x68, 0x1(, %edx, 1)
    jne     get_option_else_if_dec
    movl    $OPT_PRINT_HEX, %eax
    jmp     get_option_ret
get_option_else_if_dec:
    cmpb    $0x64, 0x1(, %edx, 1)
    jne     get_option_else_if_sign
    movl    $OPT_PRINT_DEC, %eax
    jmp     get_option_ret
get_option_else_if_sign:
    cmpb    $0x73, 0x1(, %edx, 1)
    jne     get_option_else
    movl    $OPT_USE_SIGNED, %eax
    jmp     get_option_ret
get_option_else:
    movl    $NONE_OPTION, %eax
get_option_ret:
    popl    %ebp
    ret




.globl get_optype
# OPTYPE get_optype(const char *str);
get_optype:
    pushl   %ebp
    movl    %esp, %ebp
    movl    0x8(%ebp), %edx # %edx: str

    xorl    %eax, %eax 
    movb    (%edx), %al 
    subb    $0x41, %al      # switch start from 'A'  
    cmpb    $0x19, %al      # check out of bounds
    jae     get_optype_case_default      
    jmp     *get_optype_jump_table(, %eax, 4)

get_optype_ret:
    popl    %ebp
    ret

get_optype_case_N:
get_optype_case_O:
get_optype_case_T:
    movl    $OPTYPE_UNARY, %eax 
    jmp     get_optype_ret
get_optype_case_A:
get_optype_case_I:
get_optype_case_R:
get_optype_case_X:
    movl    $OPTYPE_PAIRED, %eax 
    orl     $OPTYPE_LOGICAL, %eax
    jmp     get_optype_ret
get_optype_case_D:
get_optype_case_L:
get_optype_case_M:
get_optype_case_S:
    movl    $OPTYPE_PAIRED, %eax 
    orl     $OPTYPE_ARIRHMETIC, %eax 
    jmp     get_optype_ret
get_optype_case_B:
get_optype_case_C:
get_optype_case_E:
get_optype_case_F:
get_optype_case_G:
get_optype_case_H:
get_optype_case_J:
get_optype_case_K:
get_optype_case_P:
get_optype_case_Q:
get_optype_case_U:
get_optype_case_V:
get_optype_case_W:
get_optype_case_default:
    movl    $NONE_OPTYPE, %eax 
    jmp     get_optype_ret




.globl is_unary_op
# bool is_unary_op(const char *str);
is_unary_op:
    pushl   %ebp
    movl    %esp, %ebp

    pushl   0x8(%ebp)
    call    get_optype
    addl    $0x4, %esp 
    andl    $OPTYPE_UNARY, %eax 

    popl    %ebp
    ret




.globl is_paired_op
# bool is_paired_op(const char *str);
is_paired_op:
    pushl   %ebp
    movl    %esp, %ebp

    pushl   0x8(%ebp)
    call    get_optype
    addl    $0x4, %esp 
    andl    $OPTYPE_PAIRED, %eax 

    popl    %ebp
    ret




.globl is_arithmetic
# bool is_arithmetic(const char *str);
is_arithmetic:
    pushl   %ebp
    movl    %esp, %ebp

    pushl   0x8(%ebp)
    call    get_optype
    addl    $0x4, %esp 
    andl    $OPTYPE_ARIRHMETIC, %eax 

    popl    %ebp
    ret




.globl is_logical
# bool is_logical(const char *str);
is_logical:
    pushl   %ebp
    movl    %esp, %ebp

    pushl   0x8(%ebp)
    call    get_optype
    addl    $0x4, %esp 
    andl    $OPTYPE_LOGICAL, %eax 

    popl    %ebp
    ret




.globl is_operator
# bool is_operator(const char *str);
is_operator:
    pushl   %ebp
    movl    %esp, %ebp

    ;// if (is_unary_op(str) || is_paired_op(str))
    pushl   0x8(%ebp)
    call    is_unary_op
    addl    $0x4, %esp  
    cmpl    $0, %eax 
    jnz     is_operator_ret_true
    pushl   0x8(%ebp)
    call    is_paired_op
    addl    $0x4, %esp  
    cmpl    $0, %eax 
    jnz     is_operator_ret_true
    
    ;// else if (*str >= 'A' && *str <= 'Z')
    movl    0x8(%ebp), %edx
    cmpb    $0x41, (%edx)
    jb      is_operator_ret_false
    cmpb    $0x5a, (%edx)
    ja      is_operator_ret_false
    pushl   $is_operator_string_data_unrecognized_operator
    call    fatal 
    addl    $0x4, %esp
    jmp     is_operator_ret
is_operator_ret_true:
    movl    $TRUE, %eax 
    jmp     is_operator_ret
is_operator_ret_false:
    movl    $FALSE, %eax
is_operator_ret:
    popl    %ebp
    ret

is_operator_string_data_unrecognized_operator:
    .string "Error: unrecognized operator."




.globl get_operator
# OPERATOR get_operator(const char *str);
get_operator:
    pushl   %ebp
    movl    %esp, %ebp
    movl    0x8(%ebp), %edx # %edx: str

    xorl    %eax, %eax 
    movb    (%edx), %al 
    subb    $0x41, %al      # switch start from 'A'  
    cmpb    $0x19, %al      # check out of bounds
    jae     get_operator_case_default      
    jmp     *get_operator_jump_table(, %eax, 4)
get_operator_ret:
    popl    %ebp
    ret

get_operator_case_A:
    movl    $OP_AND, %eax 
    jmp     get_operator_ret
get_operator_case_D:
    movl    $OP_DIFF, %eax 
    jmp     get_operator_ret
get_operator_case_I:
    movl    $OP_IOR, %eax 
    jmp     get_operator_ret
get_operator_case_L:
    movl    $OP_LEFT_SHIFT, %eax 
    jmp     get_operator_ret
get_operator_case_M:
    movl    $OP_MUL, %eax 
    jmp     get_operator_ret
get_operator_case_N:
    movl    $OP_NEG, %eax 
    jmp     get_operator_ret
get_operator_case_O:
    movl    $OP_ONES_COMPL, %eax 
    jmp     get_operator_ret
get_operator_case_R:
    movl    $OP_RIGHT_SHIFT_ARITH, %eax 
    jmp     get_operator_ret
get_operator_case_S:
    movl    $OP_SUM, %eax 
    jmp     get_operator_ret
get_operator_case_T:
    movl    $OP_TWOS_COMPL, %eax 
    jmp     get_operator_ret
get_operator_case_X:
    movl    $OP_XOR, %eax 
    jmp     get_operator_ret
get_operator_case_B:
get_operator_case_C:
get_operator_case_E:
get_operator_case_F:
get_operator_case_G:
get_operator_case_H:
get_operator_case_J:
get_operator_case_K:
get_operator_case_P:
get_operator_case_Q:
get_operator_case_U:
get_operator_case_V:
get_operator_case_W:
get_operator_case_default:
    movl    $NONE_OPERATOR, %eax 
    jmp     get_operator_ret




.globl is_arg
# bool is_arg(const char *str);
is_arg:
    pushl   %ebp
    movl    %esp, %ebp
    movl    0x8(%ebp), %edx     # %edx: str

    ;// if (str == NULL) { return false; }
    cmpl    $NULL, %edx 
    je      is_arg_ret_false
    ;// else if (*str == '\0') { return false; }
    cmpb    $0, (%edx)
    je      is_arg_ret_false

    ;// else if (binco_strlen(str) > 32)
    pushl   %edx 
    pushl   %ecx 
    pushl   %edx 
    call    binco_strlen
    addl    $0x4, %esp 
    popl    %ecx
    popl    %edx 
    cmpl    $0x20, %eax 
    jbe     is_arg_loop_init
    pushl   $is_arg_string_data
    call    fatal 
    addl    $0x4, %esp 
    
is_arg_loop_init:
    ;// for (char c = *str; c != '\0'; c = *(str++))
    movb    (%edx), %cl 
is_arg_loop:
    cmpb    $0, %cl 
    je      is_arg_loop_end
    ;// if (c == '0' || c == '1') { continue; }    
    cmpb    $0x30, %cl 
    je      is_arg_loop_continue
    cmpb    $0x31, %cl 
    je      is_arg_loop_continue
    jmp     is_arg_ret_false
is_arg_loop_continue:
    incl    %edx
    movb    (%edx), %cl 
    jmp     is_arg_loop
is_arg_loop_end:

    movl    $TRUE, %eax 
    jmp     is_arg_ret
is_arg_ret_false:
    movl    $FALSE, %eax 
is_arg_ret:
    popl    %ebp
    ret

is_arg_string_data:
    .string "Error: binary input string is too long. please make sure that less than 32 bits."




.globl get_arg
# unsigned get_arg(const char *str);
get_arg:
    pushl   %ebp
    movl    %esp, %ebp 
    subl    $0x8, %esp          # -0x4(%ebp): str_len
                                # -0x8(%ebp): result
    movl    0x8(%ebp), %edx     # %edx: str


    movl    $0, -0x8(%ebp)
    pushl   %edx
    pushl   %ecx
    pushl   %edx
    call    binco_strlen
    addl    $0x4, %esp 
    popl    %ecx 
    popl    %edx 
    movl    %eax, -0x4(%ebp)

    ;// for (size_t index = 0; index < str_len; index++)
    xorl    %ecx, %ecx 
get_arg_for_loop:
    cmpl    -0x4(%ebp), %ecx 
    jae     get_arg_ret

    ;// if (*(str + index) == '1') { result += 1; }
    cmpb    $0x31, (%edx, %ecx)
    jne     get_arg_for_loop_second_if_block
    incl    -0x8(%ebp)
get_arg_for_loop_second_if_block:
    movl    -0x4(%ebp), %eax 
    decl    %eax 
    cmpl    %ecx, %eax 
    jbe     get_arg_for_loop_second_if_block_end
    sall    $1, -0x8(%ebp)
get_arg_for_loop_second_if_block_end:
    incl    %ecx
    jmp     get_arg_for_loop

get_arg_ret:
    movl    -0x8(%ebp), %eax
    addl    $0x8, %esp
    popl    %ebp 
    ret




.globl print_help
# void print_help();
print_help:
    pushl   %ebp
    movl    %esp, %ebp

    pushl   $print_help_string_data
    call    binco_strlen
    addl    $0x4, %esp
    pushl   %eax 
    pushl   $print_help_string_data
    pushl   $STDERR
    call    binco_write
    addl    $0xc, %esp 

    popl    %ebp
    ret

print_help_string_data: 
    .string "Usage:\n\t$ binco [<option>] [<operator>] <arg1> [<arg2>]\nThe options are as follows:\n\t-h: print hex\n\t-d: print decimal\n\t-s: use signed arithmetic\n\t-?: Show the user how to use the program, print help output to stderr\n\nOperator is one of the following:\n\tA - And\n\tD - Diff(erence)\n\tI - Inclusive OR\n\tL - Shift Left the number of bits specified by the second argument\n\tM - Multiply\n\tN - Negate (unary Minus)\n\tO - One's Complement (unary op)\n\tR - Arithmetic shift right the number of bits specified by the second argument\n\tS - Sum\n\tT - Two's Complement (unary op)\n\tX - Exclusive OR\n"





.globl show 
# void show(CMD_ARG *cmd_arg, COMPUTE_RESULT *compute_result);
show:
    pushl   %ebp
    movl    %esp, %ebp
    movl    0xc(%ebp), %edx     # %edx: compute_result
    movl    0x8(%ebp), %ecx     # %ecx: cmd_arg

    ;// if (cmd_arg->option & NONE_OPTION) { show_bin(compute_result); }
    testl   $NONE_OPTION, 0xc(%ecx)
    jz      show_if_else_block_2
    pushl   %edx
    pushl   %ecx 
    pushl   %edx 
    call    show_bin
    addl    $0x4, %esp
    popl    %ecx 
    popl    %edx 
    jmp     show_if_else_block_end
show_if_else_block_2:
    ;// else if (cmd_arg->option & OPT_PRINT_HEX) { show_hex(compute_result); }
    testl   $OPT_PRINT_HEX, 0xc(%ecx)
    jz      show_if_else_block_3
    pushl   %edx
    pushl   %ecx 
    pushl   %edx 
    call    show_hex
    addl    $0x4, %esp
    popl    %ecx 
    popl    %edx 
    jmp     show_if_else_block_end
show_if_else_block_3:
    ;// else if (cmd_arg->option & OPT_PRINT_DEC) { show_dec(compute_result); }
    testl   $OPT_PRINT_DEC, 0xc(%ecx)
    jz      show_if_else_block_end
    pushl   %edx
    pushl   %ecx 
    pushl   %edx 
    call    show_dec
    addl    $0x4, %esp
    popl    %ecx 
    popl    %edx 
show_if_else_block_end:
    ;// show_flag(compute_result);
    pushl   %edx
    pushl   %ecx 
    pushl   %edx 
    call    show_flag
    addl    $0x4, %esp
    popl    %ecx 
    popl    %edx 
    ;// binco_write(STDOUT, show_string_data_linefeed, binco_strlen(show_string_data_linefeed));
    pushl   %edx 
    pushl   %ecx 
    pushl   $show_string_data_linefeed
    call    binco_strlen
    addl    $0x4, %esp 
    popl    %ecx 
    popl    %edx 

    pushl   %edx 
    pushl   %ecx 
    pushl   %eax 
    pushl   $show_string_data_linefeed
    pushl   $STDOUT
    call    binco_write
    addl    $0xc, %esp 
    popl    %ecx
    popl    %edx 

    popl    %ebp
    ret
show_string_data_linefeed:
    .string "\n"




.globl show_bin
# void show_bin(COMPUTE_RESULT *compute_result);
show_bin:
    pushl   %ebp
    movl    %esp, %ebp
    subl    $0x24, %esp         
    pushl   %ebx
    movl    0x8(%ebp), %ebx     # %ebx: compute_result
    leal    -0x24(%ebp), %edx   # %edx: buffer        

    ;// binco_memset(buffer, 0, 33);
    pushl   %edx
    pushl   %ecx 
    pushl   $0x21
    pushl   $0
    pushl   %edx 
    call    binco_memset
    addl    $0xc, %esp
    popl    %ecx 
    popl    %edx 

    ;// for (int index = 31; index >= 0; index--)
    movl    $0x1f, %ecx
show_bin_loop:
    cmpl    $0, %ecx 
    jl      show_bin_loop_end
    ;// *(buffer + index) = compute_result->result & 0x00000001 ? '1' : '0';
    testl   $0x00000001, (%ebx)
    jz      show_bin_loop_else
    movb    $0x31, (%edx, %ecx)
    jmp     show_bin_loop_else_end
show_bin_loop_else:
    movb    $0x30, (%edx, %ecx)
show_bin_loop_else_end:
    ;// compute_result->result >>= 1;
    shrl    $1, (%ebx)
    decl    %ecx 
    jmp     show_bin_loop
show_bin_loop_end:

    ;// binco_write(STDOUT, buffer + 32 - mask2len(compute_result->mask), binco_strlen(buffer + 32 - mask2len(compute_result->mask)));
    pushl   %edx 
    pushl   %ecx 
    pushl   0x4(%ebx)
    call    mask2len
    addl    $0x4, %esp
    popl    %ecx 
    popl    %edx 

    movl    %edx, %ecx 
    addl    $0x20, %ecx 
    subl    %eax, %ecx      # %ecx: buffer + 32 - mask2len(compute_result->mask) 

    pushl   %edx 
    pushl   %ecx 
    pushl   %ecx 
    call    binco_strlen
    addl    $0x4, %esp 
    popl    %ecx 
    popl    %edx 

    pushl   %edx 
    pushl   %ecx 
    pushl   %eax 
    pushl   %ecx 
    pushl   $STDOUT
    call    binco_write
    addl    $0xc, %esp 
    popl    %ecx 
    popl    %edx 

    popl    %ebx
    addl    $0x24, %esp 
    popl    %ebp
    ret



.globl show_hex
# void show_hex(COMPUTE_RESULT *compute_result);
show_hex:
    pushl   %ebp
    movl    %esp, %ebp
    subl    $0x24, %esp         
    pushl   %ebx
    movl    0x8(%ebp), %ebx     # %ebx: compute_result
    leal    -0x24(%ebp), %edx   # %edx: buffer   
                                # -0x4(%ebp): sign_bit     

    ;// binco_memset(buffer, 0, 33);
    pushl   %edx
    pushl   %ecx 
    pushl   $0x21
    pushl   $0
    pushl   %edx 
    call    binco_memset
    addl    $0xc, %esp
    popl    %ecx 
    popl    %edx 

    ;// unsigned sign_bit = (compute_result->result & 0x80000000) >> 31;
    movl    (%ebx), %eax 
    andl    $0x80000000, %eax 
    shrl    $0x1f, %eax 
    movb    %al, -0x1(%ebp)

    ;// if (sign_bit && (compute_result->flag & FLAG_SIGNED))
    cmpl    $0, %eax 
    je      show_hex_loop_init
    testl   $FLAG_SIGNED, 0x8(%ebx)
    jz      show_hex_loop_init
    ;// compute_result->result ^= 0xffffffff;
    ;// compute_result->result += 1;
    andl    $0xffffffff, (%ebx)    
    incl    (%ebx)

show_hex_loop_init:
    ;// for (index = 31; index >= 0; index--)
    movl    $0x1f, %ecx 
show_hex_loop:
    cmpl    $0, %ecx
    jl      show_hex_loop_end

    ;// unsigned temp = compute_result->result % 16;
    movl    (%ebx), %eax 
    andl    $0x0000000f, %eax 

    ;// if (temp <= 9) { *(buffer + index) = '0' + temp; }
    cmpl    $0x9, %eax 
    ja      show_hex_loop_else_block
    movb    $0x30, (%edx, %ecx)
    addb    %al, (%edx, %ecx)
    jmp     show_hex_loop_else_block_end
show_hex_loop_else_block:
    ;// else { *(buffer + index) = 'A' + temp - 10; }
    movb    $0x41, (%edx, %ecx)
    addb    %al, (%edx, %ecx)
    subb    $0xa, (%edx, %ecx)
show_hex_loop_else_block_end:

    ;// compute_result->result /= 16;
    shrl    $0x4, (%ebx)
    ;// if (compute_result->result == 0) { break; }
    cmpl    $0, (%ebx)
    je      show_hex_loop_end

    decl    %ecx 
    jmp     show_hex_loop
show_hex_loop_end:

    ;// *(buffer + (--index)) = 'x';
    ;// *(buffer + (--index)) = '0';
    decl    %ecx 
    movb    $0x78, (%edx, %ecx)
    decl    %ecx 
    movb    $0x30, (%edx, %ecx)

    ;// if (sign_bit && (compute_result->flag & FLAG_SIGNED))
    cmpl    $0, -0x4(%ebp)
    je      show_hex_binco_write
    testl   $FLAG_SIGNED, 0x8(%ebx)
    jz      show_hex_binco_write
    ;// *(buffer + (--index)) = '-';
    decl    %ecx 
    movb    $0x2d, (%edx, %ecx)

show_hex_binco_write:
    ;// binco_write(STDOUT, buffer + index, binco_strlen(buffer + index));
    pushl   %edx 
    pushl   %ecx 
    pushl   %edx 
    addl    %ecx, (%esp)
    call    binco_strlen
    addl    $0x4, %esp 
    popl    %ecx 
    popl    %edx 

    pushl   %edx 
    pushl   %ecx 
    pushl   %eax 
    pushl   %edx 
    addl    %ecx, (%esp)
    pushl   $STDOUT
    call    binco_write
    addl    $0xc, %esp
    popl    %ecx 
    popl    %edx 

    popl    %ebx
    addl    $0x24, %esp 
    popl    %ebp
    ret



.extern dummy
.globl show_dec
# void show_dec(COMPUTE_RESULT *compute_result);
show_dec:
    pushl   %ebp
    movl    %esp, %ebp
    subl    $0x24, %esp         
    pushl   %ebx
    movl    0x8(%ebp), %ebx     # %ebx: compute_result
    leal    -0x24(%ebp), %edx   # %edx: buffer   

    ;// binco_memset(buffer, 0, 33);
    pushl   %edx
    pushl   %ecx 
    pushl   $0x21
    pushl   $0
    pushl   %edx 
    call    binco_memset
    addl    $0xc, %esp
    popl    %ecx 
    popl    %edx 

    ;// if (compute_result->flag & FLAG_SIGNED)
    testl   $FLAG_SIGNED, 0x8(%ebx)
    jz      show_dec_first_if_block_end
    ;// compute_result->mask = len2mask(mask2len(compute_result->mask) - 1);
    pushl   %edx
    pushl   %ecx 
    pushl   0x4(%ebx)
    call    mask2len
    addl    $0x4, %esp 
    popl    %ecx
    popl    %edx 

    decl    %eax 
    pushl   %edx 
    pushl   %ecx 
    pushl   %eax 
    call    len2mask
    addl    $0x4, %esp
    popl    %ecx
    popl    %edx
    movl    %eax, 0x4(%ebx)
show_dec_first_if_block_end:

    ;// compute_result->result &= compute_result->mask;
    movl    0x4(%ebx), %eax
    andl    %eax, (%ebx)

    ;// if (compute_result->result == 0)
    cmpl    $0, (%ebx)
    jne     show_dec_second_if_block_end
    ;// *(buffer) = '0'; binco_write(STDOUT, buffer, binco_strlen(buffer)); return;
    movb    $0x30, (%edx)

    pushl   %edx 
    pushl   %ecx 
    pushl   %edx 
    call    binco_strlen
    addl    $0x4, %esp 
    popl    %ecx 
    popl    %edx 

    pushl   %edx 
    pushl   %ecx 
    pushl   %eax 
    pushl   %edx 
    pushl   $STDOUT
    call    binco_write
    addl    $0xc, %esp
    popl    %ecx 
    popl    %edx 
    jmp     show_dec_ret
show_dec_second_if_block_end:

    pushl   %edx 
    pushl   %ecx 
    pushl   0x4(%ebx)
    call    mask2len
    addl    $0x4, %esp
    popl    %ecx 
    popl    %edx 
    movl    %eax, %ecx 
show_dec_loop:
    cmpl    $0, %ecx 
    jl      show_dec_loop_end

    ;// *(buffer + index) = (compute_result->result % 10) + '0';
    pushl   %edx
    pushl   %ebx 
    xorl    %edx, %edx 
    movl    (%ebx), %eax 
    movl    $0xa, %ebx 
    divl    %ebx 
    movl    %edx, %eax 
    popl    %ebx 
    popl    %edx 
    addl    $0x30, %eax 
    movb    %al, (%edx, %ecx)

    ;// compute_result->result /= 10;
    pushl   %edx
    pushl   %ebx 
    xorl    %edx, %edx 
    movl    (%ebx), %eax 
    movl    $0xa, %ebx 
    divl    %ebx 
    popl    %ebx 
    popl    %edx 
    movl    %eax, (%ebx)

    ;// if (compute_result->result == 0) { break; }
    cmpl    $0, %eax
    je      show_dec_loop_end

    decl    %ecx
    jmp     show_dec_loop
show_dec_loop_end:

    ;// if (compute_result->flag & FLAG_SIGNED)
    testl   $FLAG_SIGNED, 0x8(%ebx)
    jz      show_dec_third_if_block_end
    ;// index--; *(buffer + index) = '-';
    decl    %ecx 
    movb    $0x2d, (%edx, %ecx)
show_dec_third_if_block_end:

    ;// binco_write(STDOUT, buffer + index, binco_strlen(buffer + index));
    pushl   %edx
    pushl   %ecx 
    pushl   %edx 
    addl    %ecx, (%esp)
    call    binco_strlen
    addl    $0x4, %esp
    popl    %ecx
    popl    %edx 

    pushl   %edx 
    pushl   %ecx 
    pushl   %eax 
    pushl   %edx 
    addl    %ecx, (%esp)
    pushl   $STDOUT
    call    binco_write
    addl    $0xc, %esp
    popl    %ecx 
    popl    %edx 

show_dec_ret:
    popl    %ebx
    addl    $0x24, %esp 
    popl    %ebp
    ret




.globl show_flag
# void show_flag(COMPUTE_RESULT *compute_result);
show_flag:
    pushl   %ebp
    movl    %esp, %ebp
    movl    0x8(%ebp), %edx     # %edx: compute_result

    ;// if (compute_result->flag & FLAG_CARRY)
    testl   $FLAG_CARRY, 0x8(%edx)
    jz      show_flag_second_if_block
    ;// binco_write(STDOUT, show_flag_string_data_carry, binco_strlen(show_flag_string_data_carry));
    pushl   %edx 
    pushl   %ecx 
    pushl   $show_flag_string_data_carry
    call    binco_strlen
    addl    $0x4, %esp 
    popl    %ecx 
    popl    %edx 

    pushl   %edx 
    pushl   %ecx 
    pushl   %eax 
    pushl   $show_flag_string_data_carry
    pushl   $STDOUT
    call    binco_write
    addl    $0xc, %esp
    popl    %ecx 
    popl    %edx

show_flag_second_if_block:
    ;// if (compute_result->flag & FLAG_OVERFLOW)
    testl   $FLAG_OVERFLOW, 0x8(%edx)
    jz      show_flag_ret
    ;// binco_write(STDOUT, show_flag_string_data_overflow, binco_strlen(show_flag_string_data_overflow));
    pushl   %edx 
    pushl   %ecx 
    pushl   $show_flag_string_data_overflow
    call    binco_strlen
    addl    $0x4, %esp 
    popl    %ecx 
    popl    %edx 

    pushl   %edx 
    pushl   %ecx 
    pushl   %eax 
    pushl   $show_flag_string_data_overflow
    pushl   $STDOUT
    call    binco_write
    addl    $0xc, %esp
    popl    %ecx 
    popl    %edx

show_flag_ret:
    popl    %ebp
    ret
show_flag_string_data_carry:
    .string " C"
show_flag_string_data_overflow:
    .string " O"


.text
.LC0:
	.string	"Error: arg1 was missed."
.LC1:
	.string	"Error: you could not use \"-s\" here."
.LC2:
	.string	"Error: arg2 was missed."
.LC3:
	.string	"Error: you could not use two (or same) options."
.LC4:
	.string	"Error: you could not use \"-s\" on bit operation."
.LC5:
	.string	"Error: too many args for unary operation."
.LC6:
	.string	"Error: you could not use \"-s\" on logical operation."
.LC7:
	.string	"Syntax error: unrecognized symbol."


.globl	parser
parser:
.LFB0:
	.cfi_startproc
	pushl	%ebp
	.cfi_offset 5, -8
	pushl	%edi
	.cfi_offset 7, -12
	pushl	%esi
	.cfi_offset 6, -16
	pushl	%ebx
	.cfi_offset 3, -20
	call	__x86.get_pc_thunk.bx
	addl	$_GLOBAL_OFFSET_TABLE_, %ebx
	subl	$16, %esp
	movl	44(%esp), %ebp
	movl	36(%esp), %edi
	movl	40(%esp), %esi
	pushl	$20
	pushl	$0
	pushl	%ebp
	call	binco_memset
	addl	$16, %esp
	cmpl	$1, %edi
	je	.L12
	cmpl	$6, %edi
	ja	.L3
	movl	.L5@GOTOFF(%ebx,%edi,4), %eax
	addl	%ebx, %eax
	jmp	*%eax
.L5:
	.long	.L3@GOTOFF
	.long	.L3@GOTOFF
	.long	.L9@GOTOFF
	.long	.L8@GOTOFF
	.long	.L7@GOTOFF
	.long	.L6@GOTOFF
	.long	.L4@GOTOFF
.L9:
	subl	$12, %esp
	pushl	4(%esi)
	call	is_arg
	addl	$16, %esp
	testl	%eax, %eax
	jne	.L214
	subl	$12, %esp
	pushl	4(%esi)
	call	is_help
	addl	$16, %esp
	testl	%eax, %eax
	je	.L215
.L12:
	call	print_help
	subl	$12, %esp
	pushl	$0
	call	binco_exit
	addl	$16, %esp
.L3:
	subl	$12, %esp
	leal	.LC7@GOTOFF(%ebx), %eax
	pushl	%eax
	call	fatal
	addl	$16, %esp
.L1:
	addl	$12, %esp
	.cfi_remember_state
	popl	%ebx
	popl	%esi
	popl	%edi
	popl	%ebp
	ret
.L4:
	.cfi_restore_state
	subl	$12, %esp
	pushl	4(%esi)
	call	is_signed
	addl	$16, %esp
	testl	%eax, %eax
	je	.L40
	subl	$12, %esp
	pushl	8(%esi)
	call	is_format
	addl	$16, %esp
	testl	%eax, %eax
	je	.L40
.L43:
	subl	$12, %esp
	pushl	12(%esi)
	call	is_arithmetic
	addl	$16, %esp
	testl	%eax, %eax
	je	.L42
	subl	$12, %esp
	pushl	16(%esi)
	call	is_arg
	addl	$16, %esp
	testl	%eax, %eax
	je	.L42
	subl	$12, %esp
	pushl	20(%esi)
	call	is_arg
	addl	$16, %esp
	testl	%eax, %eax
	je	.L42
	subl	$12, %esp
	pushl	4(%esi)
	call	get_option
	movl	%eax, %edi
	popl	%eax
	pushl	8(%esi)
	call	get_option
	orl	%eax, %edi
	movl	%edi, 12(%ebp)
	popl	%edx
	pushl	12(%esi)
	call	get_operator
	movl	%eax, 16(%ebp)
	popl	%ecx
	pushl	20(%esi)
	call	binco_strlen
	movl	%eax, %edi
	popl	%eax
	pushl	16(%esi)
	call	binco_strlen
	popl	%edx
	popl	%ecx
	pushl	%edi
	pushl	%eax
	call	binco_max
	movl	%eax, (%esp)
	call	len2mask
	movl	%eax, 0(%ebp)
	popl	%edi
	pushl	16(%esi)
	call	get_arg
	movl	%eax, 4(%ebp)
	popl	%eax
	pushl	20(%esi)
	call	get_arg
	movl	%eax, 8(%ebp)
	popl	%eax
	pushl	12(%esi)
	call	get_operator
	addl	$16, %esp
	testb	$13, %al
	je	.L1
	subl	$12, %esp
	pushl	16(%esi)
.L208:
	call	binco_strlen
	movl	%eax, (%esp)
	call	len2mask
	addl	$16, %esp
	movl	%eax, 0(%ebp)
	jmp	.L1
.L8:
	subl	$12, %esp
	pushl	4(%esi)
	call	is_unary_op
	addl	$16, %esp
	testl	%eax, %eax
	je	.L17
	subl	$12, %esp
	pushl	8(%esi)
	call	is_arg
	addl	$16, %esp
	testl	%eax, %eax
	jne	.L216
.L17:
	subl	$12, %esp
	pushl	4(%esi)
	call	is_format
	addl	$16, %esp
	testl	%eax, %eax
	je	.L18
	subl	$12, %esp
	pushl	8(%esi)
	call	is_arg
	addl	$16, %esp
	testl	%eax, %eax
	jne	.L217
.L18:
	subl	$12, %esp
	pushl	4(%esi)
	call	is_signed
	addl	$16, %esp
	testl	%eax, %eax
	je	.L19
	subl	$12, %esp
	pushl	8(%esi)
	call	is_arg
	addl	$16, %esp
	testl	%eax, %eax
	jne	.L218
.L19:
	subl	$12, %esp
	pushl	4(%esi)
	call	is_paired_op
	addl	$16, %esp
	testl	%eax, %eax
	je	.L20
	subl	$12, %esp
	pushl	8(%esi)
	call	is_arg
	addl	$16, %esp
	testl	%eax, %eax
	je	.L20
	subl	$12, %esp
	leal	.LC2@GOTOFF(%ebx), %eax
	pushl	%eax
	call	fatal
	addl	$16, %esp
	jmp	.L3
.L7:
	subl	$12, %esp
	pushl	4(%esi)
	call	is_paired_op
	addl	$16, %esp
	testl	%eax, %eax
	je	.L22
	subl	$12, %esp
	pushl	8(%esi)
	call	is_arg
	addl	$16, %esp
	testl	%eax, %eax
	jne	.L219
.L22:
	subl	$12, %esp
	pushl	4(%esi)
	call	is_format
	addl	$16, %esp
	testl	%eax, %eax
	je	.L24
	subl	$12, %esp
	pushl	8(%esi)
	call	is_unary_op
	addl	$16, %esp
	testl	%eax, %eax
	jne	.L220
.L24:
	subl	$12, %esp
	pushl	4(%esi)
	call	is_signed
	addl	$16, %esp
	testl	%eax, %eax
	je	.L25
	subl	$12, %esp
	pushl	8(%esi)
	call	get_operator
	addl	$16, %esp
	testb	$11, %al
	jne	.L32
.L25:
	subl	$12, %esp
	pushl	4(%esi)
	call	is_unary_op
	addl	$16, %esp
	testl	%eax, %eax
	je	.L35
	subl	$12, %esp
	pushl	8(%esi)
	call	is_arg
	addl	$16, %esp
	testl	%eax, %eax
	jne	.L221
.L35:
	subl	$12, %esp
	pushl	4(%esi)
	call	is_option
	addl	$16, %esp
	testl	%eax, %eax
	je	.L3
.L39:
	subl	$12, %esp
	pushl	4(%esi)
	call	is_format
	addl	$16, %esp
	testl	%eax, %eax
	je	.L3
	subl	$12, %esp
	pushl	8(%esi)
	call	is_format
	addl	$16, %esp
	testl	%eax, %eax
	je	.L3
.L27:
	subl	$12, %esp
	leal	.LC3@GOTOFF(%ebx), %eax
	pushl	%eax
	call	fatal
	addl	$16, %esp
	jmp	.L3
.L6:
	subl	$12, %esp
	pushl	4(%esi)
	call	is_format
	addl	$16, %esp
	testl	%eax, %eax
	je	.L28
	subl	$12, %esp
	pushl	8(%esi)
	call	is_paired_op
	addl	$16, %esp
	testl	%eax, %eax
	jne	.L222
.L28:
	subl	$12, %esp
	pushl	4(%esi)
	call	is_signed
	addl	$16, %esp
	testl	%eax, %eax
	je	.L30
	subl	$12, %esp
	pushl	8(%esi)
	call	is_arithmetic
	addl	$16, %esp
	testl	%eax, %eax
	jne	.L223
.L30:
	subl	$12, %esp
	pushl	4(%esi)
	call	is_signed
	addl	$16, %esp
	testl	%eax, %eax
	je	.L31
	subl	$12, %esp
	pushl	12(%esi)
	call	get_operator
	addl	$16, %esp
	testb	$11, %al
	jne	.L32
.L31:
	subl	$12, %esp
	pushl	8(%esi)
	call	is_signed
	addl	$16, %esp
	testl	%eax, %eax
	je	.L33
	subl	$12, %esp
	pushl	12(%esi)
	call	get_operator
	addl	$16, %esp
	testb	$11, %al
	jne	.L32
.L33:
	subl	$12, %esp
	pushl	4(%esi)
	call	is_signed
	addl	$16, %esp
	testl	%eax, %eax
	je	.L34
	subl	$12, %esp
	pushl	8(%esi)
	call	get_optype
	addl	$16, %esp
	testb	$8, %al
	je	.L34
.L45:
	subl	$12, %esp
	leal	.LC6@GOTOFF(%ebx), %eax
	pushl	%eax
	call	fatal
	addl	$16, %esp
	jmp	.L3
.L34:
	subl	$12, %esp
	pushl	4(%esi)
	call	is_option
	addl	$16, %esp
	testl	%eax, %eax
	je	.L35
	subl	$12, %esp
	pushl	8(%esi)
	call	is_unary_op
	addl	$16, %esp
	testl	%eax, %eax
	je	.L35
	subl	$12, %esp
	pushl	12(%esi)
	call	is_arg
	addl	$16, %esp
	testl	%eax, %eax
	je	.L35
	subl	$12, %esp
	pushl	16(%esi)
	call	is_arg
	addl	$16, %esp
	testl	%eax, %eax
	je	.L35
.L36:
	subl	$12, %esp
	leal	.LC5@GOTOFF(%ebx), %eax
	pushl	%eax
	call	fatal
	addl	$16, %esp
	jmp	.L3
.L20:
	subl	$12, %esp
	pushl	4(%esi)
	call	is_option
	addl	$16, %esp
	testl	%eax, %eax
	je	.L3
	subl	$12, %esp
	pushl	4(%esi)
	call	is_format
	addl	$16, %esp
	testl	%eax, %eax
	je	.L21
	subl	$12, %esp
	pushl	8(%esi)
	call	is_format
	addl	$16, %esp
	testl	%eax, %eax
	jne	.L27
.L21:
	subl	$12, %esp
	pushl	4(%esi)
	call	is_option
	addl	$16, %esp
	testl	%eax, %eax
	je	.L3
	subl	$12, %esp
	pushl	8(%esi)
	call	is_operator
	addl	$16, %esp
	testl	%eax, %eax
	je	.L3
	jmp	.L15
.L40:
	subl	$12, %esp
	pushl	4(%esi)
	call	is_format
	addl	$16, %esp
	testl	%eax, %eax
	je	.L39
	subl	$12, %esp
	pushl	8(%esi)
	call	is_signed
	addl	$16, %esp
	testl	%eax, %eax
	jne	.L43
	jmp	.L39
.L42:
	subl	$12, %esp
	pushl	12(%esi)
	call	is_logical
	addl	$16, %esp
	testl	%eax, %eax
	je	.L44
	subl	$12, %esp
	pushl	16(%esi)
	call	is_arg
	addl	$16, %esp
	testl	%eax, %eax
	je	.L44
	subl	$12, %esp
	pushl	20(%esi)
	call	is_arg
	addl	$16, %esp
	testl	%eax, %eax
	jne	.L45
.L44:
	subl	$12, %esp
	pushl	12(%esi)
	call	is_operator
	addl	$16, %esp
	cmpl	$1, %eax
	jne	.L3
	subl	$12, %esp
	pushl	16(%esi)
	call	is_arg
	addl	$16, %esp
	testl	%eax, %eax
	je	.L3
	subl	$12, %esp
	pushl	20(%esi)
	call	is_arg
	addl	$16, %esp
	testl	%eax, %eax
	je	.L3
	jmp	.L45
.L32:
	subl	$12, %esp
	leal	.LC4@GOTOFF(%ebx), %eax
	pushl	%eax
	call	fatal
	addl	$16, %esp
	jmp	.L3
.L215:
	subl	$12, %esp
	pushl	4(%esi)
	call	is_option
	addl	$16, %esp
	testl	%eax, %eax
	jne	.L15
	subl	$12, %esp
	pushl	4(%esi)
	call	is_option
	addl	$16, %esp
	testl	%eax, %eax
	je	.L3
	subl	$12, %esp
	pushl	4(%esi)
	call	is_operator
	addl	$16, %esp
	testl	%eax, %eax
	jne	.L15
	subl	$12, %esp
	pushl	4(%esi)
	call	is_format
	addl	$16, %esp
	testl	%eax, %eax
	je	.L3
.L15:
	subl	$12, %esp
	leal	.LC0@GOTOFF(%ebx), %eax
	pushl	%eax
	call	fatal
	addl	$16, %esp
	jmp	.L3
.L219:
	subl	$12, %esp
	.cfi_remember_state
	pushl	12(%esi)
	call	is_arg
	addl	$16, %esp
	testl	%eax, %eax
	je	.L22
	subl	$12, %esp
	movl	$1, 12(%ebp)
	pushl	4(%esi)
	call	get_operator
	movl	%eax, 16(%ebp)
	popl	%edx
	pushl	12(%esi)
	call	binco_strlen
	popl	%ecx
	pushl	8(%esi)
	movl	%eax, %edi
	call	binco_strlen
	popl	%edx
	popl	%ecx
	pushl	%edi
	pushl	%eax
	call	binco_max
	movl	%eax, (%esp)
	call	len2mask
	movl	%eax, 0(%ebp)
	popl	%edi
	pushl	8(%esi)
	call	get_arg
	movl	%eax, 4(%ebp)
	popl	%eax
	pushl	12(%esi)
	call	get_arg
	movl	%eax, 8(%ebp)
	popl	%eax
	pushl	4(%esi)
	call	get_operator
	addl	$16, %esp
	testb	$13, %al
	je	.L1
	subl	$12, %esp
	pushl	8(%esi)
	jmp	.L208
.L222:
	.cfi_restore_state
	subl	$12, %esp
	pushl	12(%esi)
	call	is_arg
	addl	$16, %esp
	testl	%eax, %eax
	je	.L28
	subl	$12, %esp
	pushl	16(%esi)
	call	is_arg
	addl	$16, %esp
	testl	%eax, %eax
	je	.L28
	subl	$12, %esp
	pushl	4(%esi)
	call	get_option
.L212:
	movl	%eax, 12(%ebp)
	popl	%eax
	pushl	8(%esi)
	call	get_operator
	movl	%eax, 16(%ebp)
	popl	%eax
	pushl	16(%esi)
	call	binco_strlen
	movl	%eax, %edi
	popl	%eax
	pushl	12(%esi)
	call	binco_strlen
	popl	%edx
	popl	%ecx
	pushl	%edi
	pushl	%eax
	call	binco_max
	movl	%eax, (%esp)
	call	len2mask
	movl	%eax, 0(%ebp)
	popl	%edi
	pushl	12(%esi)
	call	get_arg
	movl	%eax, 4(%ebp)
	popl	%eax
	pushl	16(%esi)
	call	get_arg
	movl	%eax, 8(%ebp)
	popl	%eax
	pushl	8(%esi)
	call	get_operator
	addl	$16, %esp
	testb	$13, %al
	je	.L1
	subl	$12, %esp
	pushl	12(%esi)
	jmp	.L208
.L223:
	subl	$12, %esp
	.cfi_remember_state
	pushl	12(%esi)
	call	is_arg
	addl	$16, %esp
	testl	%eax, %eax
	je	.L30
	subl	$12, %esp
	pushl	16(%esi)
	call	is_arg
	addl	$16, %esp
	testl	%eax, %eax
	je	.L30
	subl	$12, %esp
	pushl	4(%esi)
	call	get_option
	orl	$1, %eax
	jmp	.L212
.L220:
	.cfi_restore_state
	subl	$12, %esp
	pushl	12(%esi)
	call	is_arg
	addl	$16, %esp
	testl	%eax, %eax
	je	.L24
	subl	$12, %esp
	pushl	4(%esi)
	call	get_option
	movl	%eax, 12(%ebp)
	popl	%eax
	pushl	8(%esi)
	call	get_operator
	movl	%eax, 16(%ebp)
	popl	%eax
	pushl	12(%esi)
	call	binco_strlen
	movl	%eax, (%esp)
	call	len2mask
	movl	%eax, 0(%ebp)
	popl	%eax
	pushl	12(%esi)
	call	get_arg
	addl	$16, %esp
	movl	%eax, 4(%ebp)
	jmp	.L1
.L218:
	subl	$12, %esp
	leal	.LC1@GOTOFF(%ebx), %eax
	pushl	%eax
	call	fatal
	addl	$16, %esp
	jmp	.L3
.L221:
	subl	$12, %esp
	pushl	12(%esi)
	call	is_arg
	addl	$16, %esp
	testl	%eax, %eax
	je	.L35
	jmp	.L36
.L217:
	subl	$12, %esp
	pushl	4(%esi)
	call	get_option
	movl	$1, 16(%ebp)
	movl	%eax, 12(%ebp)
.L209:
	popl	%eax
	pushl	8(%esi)
	call	binco_strlen
	movl	%eax, (%esp)
	call	len2mask
	movl	%eax, 0(%ebp)
	popl	%eax
	pushl	8(%esi)
	call	get_arg
	addl	$16, %esp
	movl	%eax, 4(%ebp)
	jmp	.L1
.L214:
	subl	$12, %esp
	pushl	4(%esi)
	call	binco_strlen
	movl	%eax, (%esp)
	call	len2mask
	movl	%eax, 0(%ebp)
	popl	%eax
	pushl	4(%esi)
	call	get_arg
	movl	$1, 12(%ebp)
	addl	$16, %esp
	movl	%eax, 4(%ebp)
	movl	$1, 16(%ebp)
	jmp	.L1
.L216:
	movl	$1, 12(%ebp)
	subl	$12, %esp
	pushl	4(%esi)
	call	get_operator
	movl	%eax, 16(%ebp)
	jmp	.L209
	.cfi_endproc
.LFE0:
	.size	parser, .-parser
	.section	.text.__x86.get_pc_thunk.bx,"axG",@progbits,__x86.get_pc_thunk.bx,comdat
	.globl	__x86.get_pc_thunk.bx
	.hidden	__x86.get_pc_thunk.bx
	.type	__x86.get_pc_thunk.bx, @function
__x86.get_pc_thunk.bx:
.LFB1:
	.cfi_startproc
	movl	(%esp), %ebx
	ret
	.cfi_endproc
