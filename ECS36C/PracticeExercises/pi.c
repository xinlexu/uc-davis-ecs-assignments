#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

int IsValidInput() {
    int IsValidInput = 1;
    char letter;
    do {
        scanf("%c", &letter);
        if (!isspace(letter)) {
            IsValidInput = 0;
        }
    } while (letter != '\n');
    return IsValidInput;
}

double random_point() {
    double x;
    x = -1 + ((double) rand()) / (((double) RAND_MAX) / 2);
    //LO + ((double) rand()) / (((double) RAND_MAX) / (HI - LO)) 
    return x;
}

int main(void) {
    int seed;
    double pi;
    do {
        printf("Enter the seed for the random number generator: ");
        scanf("%d", &seed);
    }
    while (!IsValidInput());

    int i, n;
    do {
        printf("Enter the number of iterations to run: ");
        scanf("%d", &n);
    }
    while (!IsValidInput() || n < 0);
     
    double x, y, dis;
    int count = 0;
    srand(seed);
    for (i = 1; i <= n; i++) {
        x = random_point();
        y = random_point();
        dis = sqrt(x * x + y * y);
        if (dis <= 1) {
            count++;
        }   
    }
    pi = (double) count * 4 / n;
    printf ("The value of pi is %.3lf.\n", pi);
    return 0;
}