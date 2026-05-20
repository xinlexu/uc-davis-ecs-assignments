#include <stdio.h>

int main() {
    int pas[40][40] = {};
    int row, i, j;
    printf("Please enter how many levels of Pascal's Triangle you would like to see: ");
    scanf("%d", &row);

    if (row == 0) {
        return 0;
    }
    
    pas[1][1] = 1;
    printf("1\n");

    for (i = 2; i <= row; i++) {
        for (j = 1; j <= i; j++) {
            pas[i][j] = pas[i-1][j-1] + pas[i-1][j];
            printf("%d ", pas[i][j]);
        }
        printf("\n");
    }
    return 0;
}