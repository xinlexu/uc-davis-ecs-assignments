#include <stdio.h>
#include <string.h>

void arrange(char s[], int a[], int n, int length);
void my_print(int a[], int l);

void my_print(int a[], int l) {
    int i;
    for (i = 0; i < l; i++) {
        printf("%d", a[i]);
    }
    printf("\n");
}

void arrange(char s[], int a[], int n, int length) {
    if (n == 0) {
        my_print(a, length);
        return;
    }
    
    if (s[0] == '0') {
        a[length - n] = 0;
        arrange(s + 1, a, n - 1, length);
        return;
    }
    if (s[0] == '1') {
        a[length - n] = 1;
        arrange(s + 1, a, n - 1, length);
        return;
    }
    
    a[length - n] = 0;
    arrange(s + 1, a, n - 1, length);
    a[length - n] = 1;
    arrange(s + 1, a, n - 1, length);
}


int main(int argc, char *argv[]) {
    char str[105] = {};
    int res[105] = {}; 
    strcpy(str, argv[1]);
    arrange(str, res, strlen(str), strlen(str));
    return 0;
}  




