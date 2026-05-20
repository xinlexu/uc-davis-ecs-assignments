#include <stdio.h>
#include <math.h>
int main(void){
    double money_borrowed;
    double annual_rate;
    double number_payment;
    double total;
    double monthly_payment;
    double cost_loan;

    printf("Please enter the amount of money you borrowed: $");
    scanf("%lf", &money_borrowed);

    printf("Please enter the annual interest rate: ");
    scanf("%lf", &annual_rate);

    printf("Please enter the number of payments to be made: ");
    scanf("%lf", &number_payment);

    total = (money_borrowed * (annual_rate/12) * number_payment) / (1 - pow( 1 + (annual_rate / 12), - number_payment)); 

    monthly_payment = total / number_payment;

    cost_loan = total -  money_borrowed;

    printf("A loan of $%.2lf with an annual interest of %.2lf payed off over %.0lf months will have monthly payments of $%.2lf.\n",money_borrowed, annual_rate, number_payment, monthly_payment);

    printf("In total you will pay $%.2lf, making the cost of your loan $%.2lf.\n",total, cost_loan); 
return 0;
}