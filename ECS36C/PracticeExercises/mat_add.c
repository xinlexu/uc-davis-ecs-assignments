#include <stdio.h>
#include <stdlib.h>

int main(void){
    int rows, columns, storage;
    int *values = NULL;
    printf("Please enter the number of rows: ");
    scanf("%d", &rows);

    printf("Please enter the number of columns: ");
    scanf("%d", &columns);

    values = (int *)malloc(rows * columns * sizeof(int));

    printf("Enter Matrix A\n");
    int i, j;
    for (i = 0; i < rows; i++) {
        for(j = 0; j < columns; j++) {
            scanf("%d", &values[i * columns + j]);
        }
    }
    
    printf("Enter Matrix B\n");
    for (i = 0; i < rows; i++) {
        for(j = 0; j < columns; j++) {
            scanf("%d", &storage);
            values[i * columns + j] += storage;
        }
    }

    printf("A + B =\n");
    for (i = 0; i < rows; i++) {
        for(j = 0; j < columns; j++) {
            printf("%d ", values[i * columns + j]);
        }
        printf("\n");
    }

    free(values);

    return 0;
}
