#include <stdio.h>
#include <string.h>

void temp(double n, char c1, char c2) {
    double res = 0;
    if (c1 >= 97 && c1 <= 122)
        c1 -= 32;
    if (c2 >= 97 && c2 <= 122)
        c2 -= 32;
    if (c1 == 'F') {
        if (c2 =='F')
            printf("%.2lf%c is %.2lf%c\n", n, c1, n, c2);
        else if (c2 == 'C') {
            res = (n - 32) * 5.0 / 9;
            printf("%.2lf%c is %.2lf%c\n", n, c1, res, c2);
        }
        else {
            res = (n - 32) * 5.0 / 9 + 273.15;
            printf("%.2lf%c is %.2lf%c\n", n, c1, res, c2);
        }
    }
    else if (c1 == 'C') {
        if (c2 =='C')
            printf("%.2lf%c is %.2lf%c\n", n, c1, n, c2);
        else if (c2 == 'F') {
            res = n * 1.8 + 32;
            printf("%.2lf%c is %.2lf%c\n", n, c1, res, c2);
        }
        else {
            res = n + 273.15;
            printf("%.2lf%c is %.2lf%c\n", n, c1, res, c2);
        }  
    }
    else {
        if (c2 =='K')
            printf("%.2lf%c is %.2lf%c\n", n, c1, n, c2);
        else if (c2 == 'F') {
            res = (n - 273.15) * 1.8 + 32;
            printf("%.2lf%c is %.2lf%c\n", n, c1, res, c2);
        }
        else {
            res = n - 273.15;
            printf("%.2lf%c is %.2lf%c\n", n, c1, res, c2);
        }  

    }
    return ;
}

void distance(double n, char c1, char c2)
{
    double res = 0;
    if (c1 >= 97 && c1 <= 122)
        c1 -= 32;
    if (c2 >= 97 && c2 <= 122)
        c2 -= 32;
    if (c1 == 'I')
    {
        if (c2 =='I')
            printf("%.2lf%c is %.2lf%c\n", n, c1, n, c2);
        else if (c2 == 'F') {
            res = n / 12;
            printf("%.2lf%c is %.2lf%c\n", n, c1, res, c2);
        }
        else if (c2 == 'Y') {
            res = n / 36;
            printf("%.2lf%c is %.2lf%c\n", n, c1, res, c2);
        }
        else {
            res = n / 63360;
            printf("%.2lf%c is %.2lf%c\n", n, c1, res, c2);
        }
    }
    else if (c1 == 'F') {
        if (c2 =='F')
            printf("%.2lf%c is %.2lf%c\n", n, c1, n, c2);
        else if (c2 == 'I') {
            res = n * 12;
            printf("%.2lf%c is %.2lf%c\n", n, c1, res, c2);
        }
        else if (c2 == 'Y') {
            res = n / 3;
            printf("%.2lf%c is %.2lf%c\n", n, c1, res, c2);
        }
        else {
            res = n / 5280;
            printf("%.2lf%c is %.2lf%c\n", n, c1, res, c2);
        }
    }
    else if (c1 == 'Y') {
        if (c2 =='Y')
            printf("%.2lf%c is %.2lf%c\n", n, c1, n, c2);
        else if (c2 == 'I') {
            res = n * 36;
            printf("%.2lf%c is %.2lf%c\n", n, c1, res, c2);
        }
        else if (c2 == 'F') {
            res = n * 3;
            printf("%.2lf%c is %.2lf%c\n", n, c1, res, c2);
        }
        else {
            res = n / 1760;
            printf("%.2lf%c is %.2lf%c\n", n, c1, res, c2);
        }
    }
    else {
        if (c2 =='M')
            printf("%.2lf%c is %.2lf%c\n", n, c1, n, c2);
        else if (c2 == 'I') {
            res = n * 63360;
            printf("%.2lf%c is %.2lf%c\n", n, c1, res, c2);
        }
        else if (c2 == 'Y') {
            res = n * 1760;
            printf("%.2lf%c is %.2lf%c\n", n, c1, res, c2);
        }
        else {
            res = n * 5280;
            printf("%.2lf%c is %.2lf%c\n", n, c1, res, c2);
        }
    }
    return ;
}

int main(void) {
    double n;
    int bl;

    printf ("Pick the type of conversion that you would like to do.\nT or t for temperature\nD or d for distance\nEnter your choice: ");

    char str[100];
    scanf ("%s", str);
    if (strlen(str) != 1){
        printf("Invalid formatting. Ending program.\n");
        return 0;
    }

    char c = str[0];
    if (!(c == 'T' || c == 't' || c == 'D' || c == 'd')){
        printf("Unknown conversion type %c chosen. Ending program.\n", c);
        return 0;
    }

    char c1, c2;
    if (c == 'T' || c == 't'){
        printf("Enter the temperature followed by its suffix (F, C, or K): ");
        bl = scanf("%lf%s", &n, str);
        if (strlen(str) != 1 || bl == 0){
            printf("Invalid formatting. Ending program.\n");
            return 0;
        }
        c1 = str[0];
        if (!(c1 == 'F' || c1 == 'C' || c1 == 'K' || \
              c1 == 'f' || c1 == 'c' || c1 == 'k')){
            printf("%c is not a valid temperature type. Ending program.\n", c1);
            return 0;
        }

        printf("Enter the new unit type (F, C, or K): ");
        scanf("%s", str);
        if (strlen(str) != 1){
            printf("Invalid formatting. Ending program.\n");
            return 0;
        }
        c2 = str[0];
        if (!(c2 == 'F' || c2 == 'C' || c2 == 'K' || \
              c2 == 'f' || c2 == 'c' || c2 == 'k')){
            printf("%c is not a valid temperature type. Ending program.\n", c2);
            return 0;
        }
        temp(n, c1, c2);
    }
    else
    {
        printf("Enter the distance followed by its suffix (I,F,Y,M): ");
        bl = scanf("%lf%s", &n, str);
        if (strlen(str) != 1 || bl == 0){
            printf("Invalid formatting. Ending program.\n");
            return 0;
        }
        c1 = str[0];
        if (!(c1 == 'I' || c1 == 'F' || c1 == 'Y' || c1 == 'M' ||\
              c1 == 'i' || c1 == 'f' || c1 == 'y' || c1 == 'm')){
            printf("%c is not a valid distance type. Ending program.\n", c1);
            return 0;
        }

        printf("Enter the new unit type (I,F,Y,M): ");
        scanf("%s", str);
        if (strlen(str) != 1){
            printf("Invalid formatting. Ending program.\n");
            return 0;
        }
        c2 = str[0];
        if (!(c2 == 'I' || c2 == 'F' || c2 == 'Y' || c2 == 'M' ||\
              c2 == 'i' || c2 == 'f' || c2 == 'y' || c2 == 'm')){
            printf("%c is not a valid distance type. Ending program.\n", c2);
            return 0;
        }   
        distance(n, c1, c2);
    }
    return 0;
}
