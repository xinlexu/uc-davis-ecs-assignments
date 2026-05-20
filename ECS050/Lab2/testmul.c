#include <stdio.h>
#include <stdlib.h>

extern int multiply(int, int);

/**
 * reference implementation of 32 bit unsigned multiply using shift and add:

int multiply(int n1, int n2)
{
    if (n1 < 0)
    {
        return -1;
    }
    else if (n2 < 0)
    {
        return -1;
    }

    // Allocate it in %edx, in other words, in this function 
    // we try to not allocate any varibales on stack nor heap.
    register int result = 0;

    for (unsigned sftamt = 0; sftamt < 31; sftamt++)
    {
        if ((n2 & 0x00000001) == 1)
        {
            result += n1 << sftamt;

            if (overflow) // overflow flag will be detected by assembly code
            {
                return -1;
            }
        }

        if (result < 0)
        {
            return -1;
        }

        n2 >>= 1;
    }

    return result;
}

 * 
*/

int main(int argc, char *argv[])
{
    printf("%d\n", multiply(atoi(argv[1]), atoi(argv[2])));
    
    return 0;
}
