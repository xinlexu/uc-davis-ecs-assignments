#include <stdio.h>
#include <stdlib.h>

extern int fibonacci(int);

/**
 * reference implementation of fibonacci -Iterative:

int fibonacci(int n)
{
    int cache[3] = {0, 1, 1};

    // we do not use any immediate operand (aka. magic number) 
    // for a bound for Fibonacci in assembly code.
    if (n == 0)
    {
        return cache[0];
    }
    else if (n == 1)
    {
        return cache[1];
    }

    // we treat the negative number n as a big (greater than 0x7fffffff) unsigned integer, 
    // so when overflow occurs, return -1 will be the correct behivior.
    for (unsigned i = 3; i <= *((unsigned*)&n); i++)
    {
        cache[0] = cache[1];
        cache[1] = cache[2];
        cache[2] = cache[0] + cache[1];

        if (overflow) // overflow flag will be detected by assembly code
        {
            return -1;
        }
    }

    return cache[2];
}

*/

int main(int argc, char *argv[])
{
    printf("%d\n", fibonacci(atoi(argv[1])));
    
    return 0;
}
