#include <stdio.h>
#include <string.h>
int main(void) {

    char str[100], c;
    printf("Enter the grade you want in the class: ");
    scanf("%s", str);
    if (strlen(str) != 1){
        printf("Invalid formatting. Ending program.\n");
        return 0;
    }
    c = str[0];
    if (!((c >= 'A' && c <= 'D') || (c >= 'a' && c <= 'd') || c == 'F' || c == 'f')) {
        printf("Unknown Grade Received: %c. Ending program.\n", c);
        return 0;
    }

    double score;
    printf("Enter your current percent in the class: ");
    if (scanf("%lf", &score) != 1) {
        printf("Invalid formatting. Ending program.\n");
        return 0;
    }
    if (score < 0) {
        printf("The number you last entered should have been positive. Ending program.\n");
        return 0;
    }

    double final;
    printf("Enter the weight of the final: ");
    if (scanf("%lf", &final) != 1 || scanf("%s", str) != EOF ) {
        printf("Invalid formatting. Ending program.\n");
        return 0;
    }
    if (final < 0) {
        printf("The number you last entered should have been positive. Ending program.\n");
        return 0;
    }

    double goal = 0;
    switch (c) {
        case 'a': goal = 0.90; break;
        case 'A': goal = 0.90; break;
        case 'b': goal = 0.80; break;
        case 'B': goal = 0.80; break;
        case 'c': goal = 0.70; break;
        case 'C': goal = 0.70; break;
        case 'd': goal = 0.60; break;
        case 'D': goal = 0.60; break;
        case 'f': goal = 0.00; break;
        case 'F': goal = 0.00; break;

    }
    double least = (goal * 100 - score * (1 - final/ 100)) / final;
    if (least < 0) {
        printf("You cannot score low enough on the final to get a %c in the class.\n", c);
    }
    else if (least * 100 > 100) {
        printf("You cannot get a %c in the class because you would need to score at least %.2lf%% on the final.\n", c, least * 100);
    }
    else {
        printf("You need a grade of at least %.2lf%% on the final to get a %c in the class.\n", least * 100, c);
    }
    return 0;
}
