#include <string.h>
#include <stdio.h>
#include <ctype.h>


void changeLowercase(char s[]) {
    int i;
    for (i = 0; i < strlen(s); i++) {
        if (isupper(s[i])) {
            s[i] += 32;
        }
    }
    return ;
}

void esort(char s[]) {
    char c;
    int i, j;
    for (i = 0; i < strlen(s); i++) {
        for (j = i + 1; j < strlen(s); j++) {
            if (s[i] > s[j]) {
                c = s[i];
                s[i] = s[j];
                s[j] = c;
            }
        }
    }
    return ;
}

int main() {

    char first_word[25];
    printf("Please enter the first word: ");
    scanf("%s", first_word);

    char second_word[25];
    printf("Please enter the second word: ");
    scanf("%s", second_word);
    
    char old_1stword[25];
    char old_2ndword[25];
    strcpy(old_1stword, first_word);
    strcpy(old_2ndword, second_word);

    changeLowercase(first_word);
    changeLowercase(second_word);

    esort(first_word);
    esort(second_word);

    if (strcmp(first_word, second_word) == 0) {
        printf("%s is an anagram of %s\n", old_1stword, old_2ndword);
    }
    else {
        printf("%s is NOT an anagram of %s\n", old_1stword, old_2ndword);
    }
    
    return 0;
}
