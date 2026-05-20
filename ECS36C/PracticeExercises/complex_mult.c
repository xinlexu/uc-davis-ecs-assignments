#include <stdio.h>

int main(void) {

    double a1, b1, a2, b2, c1, c2;
    char i;
    char sign;

    printf("Enter the first complex number in the form ai + b: ");
    scanf("%lf", &b1);
    scanf("%1s", &i);
    scanf("%1s", &sign);
    scanf("%lf", &a1);
   
    printf("Enter the second complex number in the form ai + b: ");
    scanf("%lf", &b2);
    scanf("%1s", &i);
    scanf("%1s", &sign);
    scanf("%lf", &a2);
   
    c1 = a1 * a2 - b1 * b2;
    c2 = a1 * b2 + a2 * b1;
   
    printf("(%.0lfi + %.0lf) * (%.0lfi + %.0lf) = %.0lfi + %.0lf\n", b1, a1, b2, a2, c2, c1);

    return 0;
}
