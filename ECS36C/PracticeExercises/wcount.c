#include <stdio.h>

int main(int argc, char *argv[]) {

    FILE *fin = fopen(argv[1], "r");
    char str[1005];
    int count = 0;
    while (fscanf(fin, "%s", str) != EOF) {
        count++;
    }
    printf("There are %d word(s).\n", count);
    return 0;
}
