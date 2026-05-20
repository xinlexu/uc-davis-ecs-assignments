#include <stdio.h>
int main(void) {
    int numWithdraw;
    int numHundred;
    int numFifty;
    int numTwenty;
    int numTen;
    int numFive;
    int numOne;
    printf("Please enter the amount of money you wish to withdraw: ");
    scanf("%d",&numWithdraw);

    numHundred = numWithdraw / 100;
    numWithdraw = numWithdraw % 100;

    numFifty = numWithdraw / 50;
    numWithdraw = numWithdraw % 50;

    numTwenty = numWithdraw / 20;
    numWithdraw = numWithdraw % 20;

    numTen = numWithdraw / 10;
    numWithdraw = numWithdraw % 10;

    numFive = numWithdraw / 5;
    numWithdraw = numWithdraw % 5;
    numOne = numWithdraw / 1;


    printf("You received %d hundred(s)\n",numHundred);
    printf("You received %d fifty(s)\n",numFifty);
    printf("You received %d twenty(s)\n",numTwenty);
    printf("You received %d ten(s)\n",numTen);
    printf("You received %d five(s)\n",numFive);
    printf("You received %d one(s)\n",numOne);
    return 0;
}