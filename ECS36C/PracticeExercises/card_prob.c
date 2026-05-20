#include <stdio.h>

double combinatorial_number(double n, double m) {
	// get C(n m) 
    double cum = 1;
    int i;
    for (i = 0; i < m; i++) {
        cum *= n - i;
    }
    
    for ( i = 1; i <= m; i++) {
        cum /= i;
    }
    return cum;
}

int main() {
    double total, copies, hand, mulliganing, turn;

    printf("Enter how many total cards there are in the deck: ");
    scanf("%lf", &total);

    printf("Enter how many copies of the card that you are looking for are in the deck: ");
    scanf("%lf", &copies);

    printf("Enter your initial hand size: ");
	scanf("%lf", &hand);

    printf("Enter how many cards you are mulliganing: ");
	scanf("%lf", &mulliganing);

    printf("Enter what turn you want to draw the card by: ");
    scanf("%lf", &turn);

    double p_phase1, p_phase2, p_success;
    
    p_phase1 = combinatorial_number(total - copies, hand + mulliganing) / combinatorial_number(total, hand + mulliganing);

    p_phase2 = combinatorial_number(total - hand - copies, turn ) / combinatorial_number(total - hand, turn);
  
    p_success = 1 - p_phase1 * p_phase2;

	printf("The probability of drawing at least one of the cards by turn %.0lf given you mulliganed %.0lf cards is %.2lf\n", turn, mulliganing, p_success);

    return 0;
}
