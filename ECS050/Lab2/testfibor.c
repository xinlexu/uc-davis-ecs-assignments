#include <stdio.h>
#include <stdlib.h>

extern int fibonacci(int);

/**
 * reference implementation of fibonacci -Recursive:

int fibonacci(int n)
{
    // we do not use any immediate operand (aka. magic number) 
    // for a bound for Fibonacci in assembly code.
    if (n < 0)
    {
        return -1;
    }
    else if (n == 0)
    {
        return 0;
    }
    else if (n == 1)
    {
        return 1;
    }
    else if (n == 2)
    {
        return 1;
    }
    else
    {
        int result = fibonacci(n - 1);
        
        if (result == -1)
        {
            return -1;
        }

        // if fibonacci(n-1) has not overflow, fibonacci(n-2) will also not overflow.
        result += fibonacci(n - 2);

        // however, we need to make sure that fibonacci(n-1) + fibonacci(n-2) 
        // will not overflow yet.
        if (overflow) // overflow flag will be detected by assembly code
        {
            return -1;
        }

        return result;
    }
}
 
 */

int main(int argc, char *argv[])
{
    printf("%d\n", fibonacci(atoi(argv[1])));

    return 0;
}
