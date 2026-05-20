/**
 * some constant values defined by macro
 */
#define NULL 0
#define TRUE 1
#define FALSE 0
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
#define STDOUT 1
#define STDERR 2
#define true 1
#define false 0
#define SYS_exit 1
#define SYS_write 4

/**
 * because we do not use any C standard headers,
 * so we define these types for the convenience
 */
typedef unsigned long size_t;
typedef signed long ssize_t;
typedef unsigned long bool;

typedef enum OPTION
{
    NONE_OPTION = 0x01,
    OPT_PRINT_HEX = 0x02,
    OPT_PRINT_DEC = 0x04,
    OPT_USE_SIGNED = 0x08,
    OPT_SHOW_HELP = 0x10,
} OPTION;

typedef enum OPERATOR
{
    NONE_OPERATOR = 1,
    OP_AND,
    OP_DIFF,
    OP_IOR,
    OP_LEFT_SHIFT,
    OP_MUL,
    OP_NEG,
    OP_ONES_COMPL,
    OP_RIGHT_SHIFT_ARITH,
    OP_SUM,
    OP_TWOS_COMPL,
    OP_XOR
} OPERATOR;

typedef enum OPTYPE
{
    NONE_OPTYPE = 0x00,
    OPTYPE_UNARY = 0x01,
    OPTYPE_PAIRED = 0x02,
    OPTYPE_ARIRHMETIC = 0x04,
    OPTYPE_LOGICAL = 0x08,
} OPTYPE;

typedef enum FLAG
{
    FLAG_OVERFLOW = 0x01,
    FLAG_CARRY = 0x02,
    FLAG_SIGNED = 0x04,
} FLAG;

/**
 * we can get these messages from command line arguments.
 * these variables will be allocated in .data fragment.
 *
 * note that option == 0 means no options detected.
 * and operator == 0 means no operators detected.
 * and flag == 0 means no flags generated.
 */
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

void binco_write(int fd, const void *buf, size_t count);
void binco_exit(int status);
size_t binco_strlen(const char *s);
void binco_memset(void *s, int c, size_t n);
void fatal(const char *str);
unsigned binco_max(unsigned a, unsigned b);
unsigned len2mask(unsigned len);
unsigned mask2len(unsigned mask);

void parser(int argc, char *argv[], CMD_ARG *cmd_arg);
bool is_help(const char *str);
bool is_format(const char *str);
bool is_signed(const char *str);
bool is_option(const char *str);
OPTION get_option(const char *str);
OPTYPE get_optype(const char *str);
bool is_unary_op(const char *str);
bool is_paired_op(const char *str);
bool is_arithmetic(const char *str);
bool is_logical(const char *str);
bool is_operator(const char *str);
OPERATOR get_operator(const char *str);
bool is_arg(const char *str);
unsigned get_arg(const char *str);
void print_help();

void calculate(CMD_ARG *cmd_arg, COMPUTE_RESULT *compute_result);
void And(CMD_ARG *cmd_arg, COMPUTE_RESULT *compute_result);
void Diff(CMD_ARG *cmd_arg, COMPUTE_RESULT *compute_result);
void InclusiveOR(CMD_ARG *cmd_arg, COMPUTE_RESULT *compute_result);
void LeftShift(CMD_ARG *cmd_arg, COMPUTE_RESULT *compute_result);
void Multiply(CMD_ARG *cmd_arg, COMPUTE_RESULT *compute_result);
void Negate(CMD_ARG *cmd_arg, COMPUTE_RESULT *compute_result);
void OnesComplement(CMD_ARG *cmd_arg, COMPUTE_RESULT *compute_result);
void RightShiftArithmetic(CMD_ARG *cmd_arg, COMPUTE_RESULT *compute_result);
void Sum(CMD_ARG *cmd_arg, COMPUTE_RESULT *compute_result);
void TwosComplement(CMD_ARG *cmd_arg, COMPUTE_RESULT *compute_result);
void XOR(CMD_ARG *cmd_arg, COMPUTE_RESULT *compute_result);
void calculate_none_operator(CMD_ARG *cmd_arg, COMPUTE_RESULT *compute_result);

void show(CMD_ARG *cmd_arg, COMPUTE_RESULT *compute_result);
void show_bin(COMPUTE_RESULT *compute_result);
void show_hex(COMPUTE_RESULT *compute_result);
void show_dec(COMPUTE_RESULT *compute_result);
void show_flag(COMPUTE_RESULT *compute_result);


void dummy()
{
    return;
}

// void binco_write(int fd, const void *buf, size_t count)
// {
//     __asm__("int $0x80\n\t" ::"d"(count), "c"(buf), "b"(fd), "a"(SYS_write)
//             :);
// }

// void binco_exit(int status)
// {
//     __asm__ __volatile__("int $0x80\n\t" ::"b"(status), "a"(SYS_exit)
//                          :);
// }

// size_t binco_strlen(const char *s)
// {
//     size_t len = 0;
//     while (*(s + len) != '\0')
//     {
//         len++;
//     }
//     return len;
// }

// void binco_memset(void *s, int c, size_t n)
// {
//     for (size_t i = 0; i < n; i++)
//     {
//         *((char*)s + i) = (char)c;
//     }
// }

// void fatal(const char *str)
// {
//     const static char *fatal_string_data_linefeed = "\n"; // 2
//     binco_write(STDERR, str, binco_strlen(str));
//     binco_write(STDERR, fatal_string_data_linefeed, binco_strlen(fatal_string_data_linefeed));
//     binco_exit(EXIT_FAILURE);
// }

// unsigned binco_max(unsigned a, unsigned b)
// {
//     return a > b ? a : b;
// }

// unsigned len2mask(unsigned len)
// {
//     unsigned result;

//     for (result = 0; (int)len > 0; len--)
//     {
//         result <<= 1;
//         result++;
//     }
//     return result;
// }

// unsigned mask2len(unsigned mask)
// {
//     unsigned result;

//     for (result = 0; mask > 0 && result < 32; result++)
//     {
//         mask >>= 1;
//     }

//     return result;
// }

// int main(int argc, char *argv[])
// {
//     CMD_ARG cmd_arg;
//     COMPUTE_RESULT compute_result;
//     parser(argc, argv, &cmd_arg);
//     calculate(&cmd_arg, &compute_result);
//     show(&cmd_arg, &compute_result);
//     return 0;
// }


// void calculate(CMD_ARG *cmd_arg, COMPUTE_RESULT *compute_result)
// {
//     binco_memset(compute_result, 0, sizeof(COMPUTE_RESULT));

//     switch (cmd_arg->operator)
//     {
//     case NONE_OPERATOR:
//         calculate_none_operator(cmd_arg, compute_result);
//         break;
//     case OP_AND:
//         And(cmd_arg, compute_result);
//         break;
//     case OP_DIFF:
//         Diff(cmd_arg, compute_result);
//         break;
//     case OP_IOR:
//         InclusiveOR(cmd_arg, compute_result);
//         break;
//     case OP_LEFT_SHIFT:
//         LeftShift(cmd_arg, compute_result);
//         break;
//     case OP_MUL:
//         Multiply(cmd_arg, compute_result);
//         break;
//     case OP_NEG:
//         Negate(cmd_arg, compute_result);
//         break;
//     case OP_ONES_COMPL:
//         OnesComplement(cmd_arg, compute_result);
//         break;
//     case OP_RIGHT_SHIFT_ARITH:
//         RightShiftArithmetic(cmd_arg, compute_result);
//         break;
//     case OP_SUM:
//         Sum(cmd_arg, compute_result);
//         break;
//     case OP_TWOS_COMPL:
//         TwosComplement(cmd_arg, compute_result);
//         break;
//     case OP_XOR:
//         XOR(cmd_arg, compute_result);
//         break;
//     default:
//         const static char *calculate_string_data = "Error: bad operation reached in calculator()";
//         fatal(calculate_string_data);
//     }
// }

// void And(CMD_ARG *cmd_arg, COMPUTE_RESULT *compute_result)
// {
//     compute_result->result = cmd_arg->arg1 & cmd_arg->arg2;
//     compute_result->mask = cmd_arg->mask;
// }

// void Diff(CMD_ARG *cmd_arg, COMPUTE_RESULT *compute_result)
// {
//     const static char *Diff_string_data_unsigned_lesser = "Error: arg1 lesser than arg2 when opearting unsigned different.";

//     if (cmd_arg->option & OPT_USE_SIGNED)
//     {
//         // arg1 will be trated as negative number
//         if (cmd_arg->arg1 >> (mask2len(cmd_arg->mask) - 1) == 1)
//         {
//             cmd_arg->arg1 += 0xffffffff ^ cmd_arg->mask;
//         }

//         if (cmd_arg->arg2 >> (mask2len(cmd_arg->mask) - 1) == 1)
//         {
//             cmd_arg->arg2 += 0xffffffff ^ cmd_arg->mask;
//         }

//         cmd_arg->arg2 = (cmd_arg->arg2 ^ 0xffffffff) + 1;
//         Sum(cmd_arg, compute_result);
//     }
//     else
//     {
//         if (cmd_arg->arg1 < cmd_arg->arg2)
//         {
//             fatal(Diff_string_data_unsigned_lesser);
//         }

//         compute_result->result = cmd_arg->arg1 - cmd_arg->arg2;
//         compute_result->mask = cmd_arg->mask;
//     }
// }

// void InclusiveOR(CMD_ARG *cmd_arg, COMPUTE_RESULT *compute_result)
// {
//     compute_result->result = cmd_arg->arg1 | cmd_arg->arg2;
//     compute_result->mask = cmd_arg->mask;
// }

// void LeftShift(CMD_ARG *cmd_arg, COMPUTE_RESULT *compute_result)
// {
//     if (cmd_arg->arg2 >= 32)
//     {
//         compute_result->result = 0;
//         compute_result->mask = len2mask(1);
//     }
//     else
//     {
//         compute_result->result = cmd_arg->arg1 << cmd_arg->arg2;
//         compute_result->mask = len2mask(mask2len(cmd_arg->mask) + cmd_arg->arg2);

//         if (mask2len(cmd_arg->mask) + cmd_arg->arg2 >= 32)
//         {
//             compute_result->mask = len2mask(32);
//         }

//         if (cmd_arg->option & OPT_USE_SIGNED)
//         {
//             compute_result->flag |= FLAG_SIGNED;
//         }
//     }
// }

// void Multiply(CMD_ARG *cmd_arg, COMPUTE_RESULT *compute_result)
// {
//     compute_result->result = cmd_arg->arg1 * cmd_arg->arg2;

//     unsigned len = 0;
//     while (compute_result->result >> len != 0)
//     {
//         len++;
//     }

//     compute_result->mask = len2mask(len);
// }

// void Negate(CMD_ARG *cmd_arg, COMPUTE_RESULT *compute_result)
// {
//     compute_result->result = -cmd_arg->arg1;
//     compute_result->mask = cmd_arg->mask;
// }

// void OnesComplement(CMD_ARG *cmd_arg, COMPUTE_RESULT *compute_result)
// {
//     compute_result->result = cmd_arg->arg1 ^ 0xffffffff;
//     compute_result->mask = cmd_arg->mask;
// }

// void RightShiftArithmetic(CMD_ARG *cmd_arg, COMPUTE_RESULT *compute_result)
// {
//     if (cmd_arg->arg2 >= mask2len(cmd_arg->mask))
//     {
//         compute_result->result = 0;
//         compute_result->mask = len2mask(1);
//     }
//     else
//     {
//         compute_result->result = cmd_arg->arg1 >> cmd_arg->arg2;
//         compute_result->mask = len2mask(mask2len(cmd_arg->mask) - cmd_arg->arg2);
//     }
// }

// void Sum(CMD_ARG *cmd_arg, COMPUTE_RESULT *compute_result)
// {
//     unsigned result = cmd_arg->arg1 + cmd_arg->arg2;

//     // carry out for arg1 and arg2
//     unsigned msb_cout = result >> mask2len(cmd_arg->mask);

//     // carry out of second most significant bit
//     unsigned smsb_cout = ((cmd_arg->arg1 & (cmd_arg->mask >> 1)) + (cmd_arg->arg2 & (cmd_arg->mask >> 1))) >> (mask2len(cmd_arg->mask) - 1);

//     // for signed sum operation
//     if (cmd_arg->option & OPT_USE_SIGNED)
//     {
//         // carry detect
//         if (msb_cout == 1)
//         {
//             compute_result->flag |= FLAG_CARRY;
//         }

//         // overflow detect
//         if ((msb_cout == 1) ^ (smsb_cout))
//         {
//             compute_result->flag |= FLAG_OVERFLOW;
//         }

//         // sign detect
//         if (msb_cout == 1)
//         {
//             compute_result->flag |= FLAG_SIGNED;
//         }
//     }
//     // for unsigned sum operation
//     else
//     {
//         // carry detect
//         if (msb_cout == 1)
//         {
//             compute_result->flag |= FLAG_CARRY;
//         }

//         // overflow detect
//         if (smsb_cout == 0 && msb_cout == 1)
//         {
//             compute_result->flag |= FLAG_OVERFLOW;
//         }
//     }

//     compute_result->result = result;

//     unsigned result_len;
//     for (result_len = 0; result_len < 32; result_len++)
//     {
//         if (result >> result_len == 0)
//         {
//             break;
//         }
//     }

//     // for negative result
//     if (result_len == 0)
//     {
//         result_len = 1;
//     }
//     else if (result_len == 32)
//     {
//         compute_result->result = (0xffffffff ^ compute_result->result) + 1;
//         result_len = mask2len(cmd_arg->mask) + 1;
//         compute_result->flag |= FLAG_SIGNED;
//     }

//     compute_result->mask = len2mask(result_len);
// }

// void TwosComplement(CMD_ARG *cmd_arg, COMPUTE_RESULT *compute_result)
// {
//     compute_result->result = (cmd_arg->arg1 ^ 0xffffffff) + 1;
//     compute_result->mask = cmd_arg->mask;
// }

// void XOR(CMD_ARG *cmd_arg, COMPUTE_RESULT *compute_result)
// {
//     compute_result->result = cmd_arg->arg1 ^ cmd_arg->arg2;
//     compute_result->mask = cmd_arg->mask;
// }

// void calculate_none_operator(CMD_ARG *cmd_arg, COMPUTE_RESULT *compute_result)
// {
//     compute_result->result = cmd_arg->arg1;
//     compute_result->mask = cmd_arg->mask;
// }

// bool is_help(const char *str)
// {
//     if (*(str) == '-' && *(str + 1) == '?' && *(str + 2) == '\0')
//     {
//         return true;
//     }
//     else
//     {
//         return false;
//     }
// }

// bool is_format(const char *str)
// {
//     if (*(str) == '-' && (*(str + 1) == 'h' || *(str + 1) == 'd') && *(str + 2) == '\0')
//     {
//         return true;
//     }
//     else
//     {
//         return false;
//     }
// }

// bool is_signed(const char *str)
// {
//     if (*(str) == '-' && *(str + 1) == 's' && *(str + 2) == '\0')
//     {
//         return true;
//     }
//     else
//     {
//         return false;
//     }
// }

// bool is_option(const char *str)
// {
//     if (is_help(str) || is_format(str) || is_signed(str))
//     {
//         return true;
//     }
//     else if (*str == '-')
//     {
//         const static char *is_option_string_data = "Error: Bad option.";
//         fatal(is_option_string_data);
//     }

//     return false;
// }

// OPTION get_option(const char *str)
// {
//     if (!is_option(str))
//     {
//         return NONE_OPTION;
//     }
//     else if (*(str + 1) == '?')
//     {
//         return OPT_SHOW_HELP;
//     }
//     else if (*(str + 1) == 'h')
//     {
//         return OPT_PRINT_HEX;
//     }
//     else if (*(str + 1) == 'd')
//     {
//         return OPT_PRINT_DEC;
//     }
//     else if (*(str + 1) == 's')
//     {
//         return OPT_USE_SIGNED;
//     }
//     else
//     {
//         return NONE_OPTION;
//     }
// }

// OPTYPE get_optype(const char *str)
// {
//     switch (*str)
//     {
//     case 'N':
//     case 'O':
//     case 'T':
//         return OPTYPE_UNARY;
//     case 'A':
//     case 'I':
//     case 'R':
//     case 'X':
//         return OPTYPE_PAIRED | OPTYPE_LOGICAL;
//     case 'D':
//     case 'L':
//     case 'M':
//     case 'S':
//         return OPTYPE_PAIRED | OPTYPE_ARIRHMETIC;
//     default:
//         return NONE_OPTYPE;
//     }
// }

// bool is_unary_op(const char *str)
// {
//     return get_optype(str) & OPTYPE_UNARY;
// }

// bool is_paired_op(const char *str)
// {
//     return get_optype(str) & OPTYPE_PAIRED;
// }

// bool is_arithmetic(const char *str)
// {
//     return get_optype(str) & OPTYPE_ARIRHMETIC;
// }

// bool is_logical(const char *str)
// {
//     return get_optype(str) & OPTYPE_LOGICAL;
// }

// bool is_operator(const char *str)
// {
//     if (is_unary_op(str) || is_paired_op(str))
//     {
//         return true;
//     }
//     else if (*str >= 'A' && *str <= 'Z')
//     {
//         const static char *is_operator_string_data_unrecognized_operator = "Error: unrecognized operator.";
//         fatal(is_operator_string_data_unrecognized_operator);
//     }

//     return false;
// }

// OPERATOR get_operator(const char *str)
// {
//     switch (*str)
//     {
//     case 'A':
//         return OP_AND;
//     case 'D':
//         return OP_DIFF;
//     case 'I':
//         return OP_IOR;
//     case 'L':
//         return OP_LEFT_SHIFT;
//     case 'M':
//         return OP_MUL;
//     case 'N':
//         return OP_NEG;
//     case 'O':
//         return OP_ONES_COMPL;
//     case 'R':
//         return OP_RIGHT_SHIFT_ARITH;
//     case 'S':
//         return OP_SUM;
//     case 'T':
//         return OP_TWOS_COMPL;
//     case 'X':
//         return OP_XOR;
//     default:
//         return NONE_OPERATOR;
//     }
// }

// bool is_arg(const char *str)
// {
//     if (str == NULL)
//     {
//         return false;
//     }
//     else if (*str == '\0')
//     {
//         return false;
//     }
//     else if (binco_strlen(str) > 32)
//     {
//         const static char *is_arg_string_data = "Error: binary input string is too long. please make sure that less than 32 bits.";
//         fatal(is_arg_string_data);
//     }

//     for (char c = *str; c != '\0'; c = *(str++))
//     {
//         if (c == '0' || c == '1')
//         {
//             continue;
//         }
//         else
//         {
//             return false;
//         }
//     }

//     return true;
// }

// unsigned get_arg(const char *str)
// {
//     unsigned result = 0;

//     size_t str_len = binco_strlen(str);

//     // convert the binary to unsigned integer
//     for (size_t index = 0; index < str_len; index++)
//     {
//         if (*(str + index) == '1')
//         {
//             result += 1;
//         }

//         if (index < str_len - 1)
//         {
//             result <<= 1;
//         }
//     }

//     return result;
// }

// void print_help()
// {
//     const static char *print_help_string_data =
//         "Usage:\n\t$ binco [<option>] [<operator>] <arg1> [<arg2>]\n"
//         "\nThe options are as follows:\n"
//         "\t-h: print hex\n"
//         "\t-d: print decimal\n"
//         "\t-s: use signed arithmetic\n"
//         "\t-?: Show the user how to use the program, print help output to stderr\n"
//         "\nOperator is one of the following:\n"
//         "\tA - And\n"
//         "\tD - Diff(erence)\n"
//         "\tI - Inclusive OR\n"
//         "\tL - Shift Left the number of bits specified by the second argument\n"
//         "\tM - Multiply\n"
//         "\tN - Negate (unary Minus)\n"
//         "\tO - One's Complement (unary op)\n"
//         "\tR - Arithmetic shift right the number of bits specified by the second argument\n"
//         "\tS - Sum\n"
//         "\tT - Two's Complement (unary op)\n"
//         "\tX - Exclusive OR\n";
//     binco_write(STDERR, print_help_string_data, binco_strlen(print_help_string_data));
// }


// void show(CMD_ARG *cmd_arg, COMPUTE_RESULT *compute_result)
// {
//     if (cmd_arg->option & NONE_OPTION)
//     {
//         show_bin(compute_result);
//     }
//     else if (cmd_arg->option & OPT_PRINT_HEX)
//     {
//         show_hex(compute_result);
//     }
//     else if (cmd_arg->option & OPT_PRINT_DEC)
//     {
//         show_dec(compute_result);
//     }

//     show_flag(compute_result);

//     const static char *show_string_data_linefeed = "\n"; // 2
//     binco_write(STDOUT, show_string_data_linefeed, binco_strlen(show_string_data_linefeed));
// }

// void show_bin(COMPUTE_RESULT *compute_result)
// {
//     static char buffer[33];
//     binco_memset(buffer, 0, 33);

//     for (int index = 31; index >= 0; index--)
//     {
//         *(buffer + index) = compute_result->result & 0x00000001 ? '1' : '0';
//         compute_result->result >>= 1;
//     }

//     binco_write(STDOUT, buffer + 32 - mask2len(compute_result->mask), binco_strlen(buffer + 32 - mask2len(compute_result->mask)));
// }

// void show_hex(COMPUTE_RESULT *compute_result)
// {
//     static char buffer[33];
//     binco_memset(buffer, 0, 33);

//     unsigned sign_bit = (compute_result->result & 0x80000000) >> 31;

//     // if n is a negative integer, convert to its negative
//     if (sign_bit && (compute_result->flag & FLAG_SIGNED))
//     {
//         compute_result->result ^= 0xffffffff;
//         compute_result->result += 1;
//     }

//     // convert the non-negative binary to hex
//     int index;
//     for (index = 31; index >= 0; index--)
//     {
//         unsigned temp = compute_result->result % 16;

//         // A~F
//         if (temp <= 9)
//         {
//             *(buffer + index) = '0' + temp;
//         }
//         else
//         {
//             *(buffer + index) = 'A' + temp - 10;
//         }

//         compute_result->result /= 16;
//         if (compute_result->result == 0)
//         {
//             break;
//         }
//     }

//     // add '0x' prefix
//     *(buffer + (--index)) = 'x';
//     *(buffer + (--index)) = '0';

//     // if n is a negative integer, add a '-' sign
//     if (sign_bit && (compute_result->flag & FLAG_SIGNED))
//     {
//         *(buffer + (--index)) = '-';
//     }

//     binco_write(STDOUT, buffer + index, binco_strlen(buffer + index));
// }

// void show_dec(COMPUTE_RESULT *compute_result)
// {
//     static char buffer[33];
//     binco_memset(buffer, 0, 33);

//     if (compute_result->flag & FLAG_SIGNED)
//     {
//         compute_result->mask = len2mask(mask2len(compute_result->mask) - 1);
//     }

//     compute_result->result &= compute_result->mask;

//     if (compute_result->result == 0)
//     {
//         *(buffer) = '0';
//         binco_write(STDOUT, buffer, binco_strlen(buffer));
//         return;
//     }

//     // convert the binary to decimal
//     int index;
//     for (index = mask2len(compute_result->mask); index >= 0; index--)
//     {
//         *(buffer + index) = (compute_result->result % 10) + '0';

//         compute_result->result /= 10;
//         if (compute_result->result == 0)
//         {
//             break;
//         }
//     }

//     // if n is a negative integer, add a '-' sign
//     if (compute_result->flag & FLAG_SIGNED)
//     {
//         index--;
//         *(buffer + index) = '-';
//     }

//     binco_write(STDOUT, buffer + index, binco_strlen(buffer + index));
// }

// void show_flag(COMPUTE_RESULT *compute_result)
// {
//     if (compute_result->flag & FLAG_CARRY)
//     {
//         const static char *show_flag_string_data_carry = " C"; // 3
//         binco_write(STDOUT, show_flag_string_data_carry, binco_strlen(show_flag_string_data_carry));
//     }

//     if (compute_result->flag & FLAG_OVERFLOW)
//     {
//         const static char *show_flag_string_data_overflow = " O"; // 3
//         binco_write(STDOUT, show_flag_string_data_overflow, binco_strlen(show_flag_string_data_overflow));
//     }
// }

// void parser(int argc, char *argv[], CMD_ARG *cmd_arg)
// {
//     const static char *parser_string_data_syntax_error = "Syntax error: unrecognized symbol.";
//     const static char *parser_string_data_sign_could_not_use_here = "Error: you could not use \"-s\" here.";
//     const static char *parser_string_data_sign_on_bit_operation = "Error: you could not use \"-s\" on bit operation.";
//     const static char *parser_string_data_sign_on_logical_operation = "Error: you could not use \"-s\" on logical operation.";
//     const static char *parser_string_data_arg1_missed = "Error: arg1 was missed.";
//     const static char *parser_string_data_arg2_missed = "Error: arg2 was missed.";
//     const static char *parser_string_data_two_format = "Error: you could not use two (or same) options.";
//     const static char *parser_string_data_too_many_args = "Error: too many args for unary operation.";

//     binco_memset(cmd_arg, 0, sizeof(CMD_ARG));

//     // $ binco
//     if (argc == 1)
//     {
//         print_help();
//         binco_exit(EXIT_SUCCESS);
//     }
//     else if (argc == 2)
//     {
//         // $ binco arg1
//         if (is_arg(argv[1]))
//         {
//             cmd_arg->mask = len2mask(binco_strlen(argv[1]));
//             cmd_arg->arg1 = get_arg(argv[1]);
//             cmd_arg->option = NONE_OPTION;
//             cmd_arg->operator = NONE_OPERATOR;
//             return;
//         }
//         // $ binco help
//         else if (is_help(argv[1]))
//         {
//             print_help();
//             binco_exit(EXIT_SUCCESS);
//         }
//         // $ binco option
//         else if (is_option(argv[1]))
//         {
//             fatal(parser_string_data_arg1_missed);
//         }
//         // $ binco bad_option
//         else if (!is_option(argv[1]))
//         {
//             ; // do nothing
//         }
//         // $ binco operator
//         else if (is_operator(argv[1]))
//         {
//             fatal(parser_string_data_arg1_missed);
//         }
//         // $ binco format
//         else if (is_format(argv[1]))
//         {
//             fatal(parser_string_data_arg1_missed);
//         }
//     }
//     else if (argc == 3)
//     {
//         // $ binco unary_op arg1
//         if (is_unary_op(argv[1]) && is_arg(argv[2]))
//         {
//             cmd_arg->option = NONE_OPTION;
//             cmd_arg->operator= get_operator(argv[1]);
//             cmd_arg->mask = len2mask(binco_strlen(argv[2]));
//             cmd_arg->arg1 = get_arg(argv[2]);
//             return;
//         }
//         // $ binco format arg1
//         else if (is_format(argv[1]) && is_arg(argv[2]))
//         {
//             cmd_arg->option = get_option(argv[1]);
//             cmd_arg->operator = NONE_OPERATOR;
//             cmd_arg->mask = len2mask(binco_strlen(argv[2]));
//             cmd_arg->arg1 = get_arg(argv[2]);
//             return;
//         }
//         // $binco sign arg1
//         else if (is_signed(argv[1]) && is_arg(argv[2]))
//         {
//             fatal(parser_string_data_sign_could_not_use_here);
//         }
//         // $ binco paired_op arg1
//         else if (is_paired_op(argv[1]) && is_arg(argv[2]))
//         {
//             fatal(parser_string_data_arg2_missed);
//         }
//         // $ binco bad_option *
//         else if (!is_option(argv[1]))
//         {
//             ; // do nothing
//         }
//         // $ binco format format *
//         else if (is_format(argv[1]) && is_format(argv[2]))
//         {
//             fatal(parser_string_data_two_format);
//         }
//         // $ binco option operator
//         else if (is_option(argv[1]) && is_operator(argv[2]))
//         {
//             fatal(parser_string_data_arg1_missed);
//         }
//     }
//     else if (argc == 4)
//     {
//         // $ binco paired_op arg1 arg2
//         if (is_paired_op(argv[1]) && is_arg(argv[2]) && is_arg(argv[3]))
//         {
//             cmd_arg->option = NONE_OPTION;
//             cmd_arg->operator= get_operator(argv[1]);
//             cmd_arg->mask = len2mask(binco_max(binco_strlen(argv[2]), binco_strlen(argv[3])));
//             cmd_arg->arg1 = get_arg(argv[2]);
//             cmd_arg->arg2 = get_arg(argv[3]);

//             if (get_operator(argv[1]) & (OP_LEFT_SHIFT | OP_RIGHT_SHIFT_ARITH))
//             {
//                 cmd_arg->mask = len2mask(binco_strlen(argv[2]));
//             }
//             return;
//         }
//         // $ binco format unary_op arg1
//         else if (is_format(argv[1]) && is_unary_op(argv[2]) && is_arg(argv[3]))
//         {
//             cmd_arg->option = get_option(argv[1]);
//             cmd_arg->operator= get_operator(argv[2]);
//             cmd_arg->mask = len2mask(binco_strlen(argv[3]));
//             cmd_arg->arg1 = get_arg(argv[3]);
//             return;
//         }
//         // $ binco sign bit_op arg1
//         else if (is_signed(argv[1]) && (get_operator(argv[2]) & (OP_ONES_COMPL | OP_TWOS_COMPL)))
//         {
//             fatal(parser_string_data_sign_on_bit_operation);
//         }
//         // $ binco unary_op arg1 arg2
//         else if (is_unary_op(argv[1]) && is_arg(argv[2]) && is_arg(argv[3]))
//         {
//             fatal(parser_string_data_too_many_args);
//         }
//         // $ binco bad_option * *
//         else if (!is_option(argv[1]))
//         {
//             ; // do nothing
//         }
//         // $ binco format format *
//         else if (is_format(argv[1]) && is_format(argv[2]))
//         {
//             fatal(parser_string_data_two_format);
//         }
//     }
//     else if (argc == 5)
//     {
//         // $ binco format paired_op arg1 arg2
//         if (is_format(argv[1]) && is_paired_op(argv[2]) && is_arg(argv[3]) && is_arg(argv[4]))
//         {
//             cmd_arg->option = get_option(argv[1]);
//             cmd_arg->operator= get_operator(argv[2]);
//             cmd_arg->mask = len2mask(binco_max(binco_strlen(argv[3]), binco_strlen(argv[4])));
//             cmd_arg->arg1 = get_arg(argv[3]);
//             cmd_arg->arg2 = get_arg(argv[4]);

//             if (get_operator(argv[2]) & (OP_LEFT_SHIFT | OP_RIGHT_SHIFT_ARITH))
//             {
//                 cmd_arg->mask = len2mask(binco_strlen(argv[3]));
//             }
//             return;
//         }
//         // $ binco signed arithmetic arg1 arg2
//         else if (is_signed(argv[1]) && is_arithmetic(argv[2]) && is_arg(argv[3]) && is_arg(argv[4]))
//         {
//             cmd_arg->option = get_option(argv[1]) | NONE_OPTION;
//             cmd_arg->operator= get_operator(argv[2]);
//             cmd_arg->mask = len2mask(binco_max(binco_strlen(argv[3]), binco_strlen(argv[4])));
//             cmd_arg->arg1 = get_arg(argv[3]);
//             cmd_arg->arg2 = get_arg(argv[4]);

//             if (get_operator(argv[2]) & (OP_LEFT_SHIFT | OP_RIGHT_SHIFT_ARITH))
//             {
//                 cmd_arg->mask = len2mask(binco_strlen(argv[3]));
//             }
//             return;
//         }
//         // $ binco signed format unary_op arg1
//         else if (is_signed(argv[1]) && (get_operator(argv[3]) & (OP_ONES_COMPL | OP_TWOS_COMPL)))
//         {
//             fatal(parser_string_data_sign_on_bit_operation);
//         }
//         // $ binco format signed unary_op arg1
//         else if (is_signed(argv[2]) && (get_operator(argv[3]) & (OP_ONES_COMPL | OP_TWOS_COMPL)))
//         {
//             fatal(parser_string_data_sign_on_bit_operation);
//         }
//         // $ binco sign logical_op arg1 arg2
//         else if (is_signed(argv[1]) && (get_optype(argv[2]) & OPTYPE_LOGICAL))
//         {
//             fatal(parser_string_data_sign_on_logical_operation);
//         }
//         // $ binco option unary_op arg1 arg2
//         else if (is_option(argv[1]) && is_unary_op(argv[2]) && is_arg(argv[3]) && is_arg(argv[4]))
//         {
//             fatal(parser_string_data_too_many_args);
//         }
//         // $ binco bad_option * * *
//         else if (!is_option(argv[1]))
//         {
//             ; // do nothing
//         }
//         // $ binco format format * *
//         else if (is_format(argv[1]) && is_format(argv[2]))
//         {
//             fatal(parser_string_data_two_format);
//         }
//     }
//     else if (argc == 6)
//     {
//         // $ binco signed format * * *
//         if (((is_signed(argv[1]) && is_format(argv[2])) || (is_format(argv[1]) && is_signed(argv[2]))))
//         {
//             // $ * * * arithmetic arg1 arg2
//             if (is_arithmetic(argv[3]) && is_arg(argv[4]) && is_arg(argv[5]))
//             {
//                 cmd_arg->option = get_option(argv[1]) | get_option(argv[2]);
//                 cmd_arg->operator= get_operator(argv[3]);
//                 cmd_arg->mask = len2mask(binco_max(binco_strlen(argv[4]), binco_strlen(argv[5])));
//                 cmd_arg->arg1 = get_arg(argv[4]);
//                 cmd_arg->arg2 = get_arg(argv[5]);

//                 if (get_operator(argv[3]) & (OP_LEFT_SHIFT | OP_RIGHT_SHIFT_ARITH))
//                 {
//                     cmd_arg->mask = len2mask(binco_strlen(argv[4]));
//                 }
//                 return;
//             }
//             // $ * * * logical arg1 arg2
//             else if (is_logical(argv[3]) && is_arg(argv[4]) && is_arg(argv[5]))
//             {
//                 fatal(parser_string_data_sign_on_logical_operation);
//             }
//             // $ * * * bad_operator arg1 arg2
//             else if (is_operator(argv[3]) == NONE_OPERATOR && is_arg(argv[4]) && is_arg(argv[5]))
//             {
//                 fatal(parser_string_data_sign_on_logical_operation);
//             }
//         }
//         // $ binco format format * * *
//         else if (is_format(argv[1]) && is_format(argv[2]))
//         {
//             fatal(parser_string_data_two_format);
//         }
//     }

//     fatal(parser_string_data_syntax_error);
// }

