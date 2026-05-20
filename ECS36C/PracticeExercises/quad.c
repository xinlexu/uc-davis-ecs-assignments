#include <stdio.h>
#include <math.h>
int main(void) {
    double a, b, c;
    double d, x, x1;
    printf("Given a quadratic equation of the form a*x^2 + b * x + c\n");

    printf("Please enter a: ");
    scanf("%lf", &a);

    printf("Please enter b: ");
    scanf("%lf", &b);

    printf("Please enter c: ");
    scanf("%lf", &c);

    d = b * b - 4 * a * c;
    if (fabs(d) <= 0.01) {
        x = -b / a / 2;
        printf("There is one real solution: %.2lf\n", x);
    }
    else if (d < -0.01) {
        printf("There are no real solutions\n");
    }
    else {
        x = -b + sqrt(d);
        x /= 2 * a;

        x1 = -b - sqrt(d);
        x1 /= 2 * a;
       printf("There are 2 real solutions\nSolution 1: %.2lf\nSolution 2: %.2lf\n", x, x1);  
    }
    return 0;
}
