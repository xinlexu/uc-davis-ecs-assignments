#include <stdio.h>
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

int main() {
    double money, owe, aIR, mMP;            // minimum monthly loan payment  
    int cAge = 0;                           // current age   
    int rAge = 0;                           // retire age   
    double aRR = 0;                         // annual rate of return predicted   

    do {
        printf("Enter how much money you will be putting towards loans/retirement each month: ");
        scanf("%lf", &money);
    } while (!IsValidInput() || money < 0);

 do {
        printf("Enter how much you owe in loans: ");
        scanf("%lf", &owe);
    } while (!IsValidInput() || owe < 0);

 do {
        printf("Enter the annual interest rate of the loans: ");
  scanf("%lf", &aIR);
    } while (!IsValidInput() || aIR < 0);
    
    do {
  printf("Enter your minimum monthly loan payment: ");
  scanf("%lf", &mMP);
    } while (!IsValidInput() || mMP < 0);

 if (mMP > money) {
  printf("You didn't set aside enough money to pay off our loans. Ending program.\n");
  return 0;
 }
 
   do {
     printf("Enter your current age: ");
     scanf("%d", &cAge);
    } while (!IsValidInput() || cAge < 0); 
 
 do {
  printf("Enter the age you plan to retire at: ");
  scanf("%d", &rAge);
    } while (!IsValidInput() || rAge < cAge);

 do {
  printf("Enter the annual rate of return you predict for your investments: ");
     scanf("%lf", &aRR);
    } while (!IsValidInput() || aRR < 0);   
   
    int i;
    int month = (rAge - cAge) * 12;
    double sum1 = 0, owe1;
    double sum2 = 0, owe2;
    owe1 = owe2 = owe;
    int bl = 1;
    for (i = 1; i <= month; i++) {
        owe1 *= 1 + aIR / 12;
        sum1 *= 1 + aRR / 12;
        if (owe1 < mMP && bl) {
            bl = 0;
            mMP = 0;
            sum1 += money - owe1;
            owe1 = 0;
        }
        else {
            if (bl) {
                owe1 -= mMP;
            }
         sum1 += (money - mMP);   
        }
    }
    bl = 1;
    for (i = 1; i <= month; i++) {
        owe2 *= 1 + aIR / 12;
        sum2 *= 1 + aRR / 12;
        if (owe2 >= money) {
            owe2 -= money;
        }
        else if (bl) {
            bl = 0;
            sum2 += money - owe2;
            owe2 = 0;
        }
        else {
            sum2 += money;   
        }       
    }
    
    if (sum1 >= sum2) {
     if (owe1 != 0) {
      printf("Warning! After you retire you will still have $%.2lf in loans left.\n", owe1);
  }
     printf("You should only make the minimum payments on your loan and apply the rest towards retirement.\n");
  printf("If you do you will have $%.2lf when you retire as opposed to $%.2lf if you payed off your loan before investing.\n", sum1, sum2);
 }
       
    else {
     if (owe1 != 0) {
      printf("Warning! After you retire you will still have $%.2lf in loans left.\n", owe1);
  }
        printf("You should apply all $%.2lf towards your loan before making any investments.\n", money);
        printf("If you do you will have $%.2lf when you retire as opposed to $%.2lf if you only made minimum payments.\n", sum2, sum1);
    }    
    return 0;
}