#include <stdio.h>
#include <math.h>

typedef struct Point_struct {
    int x;
    int y; 
} Point;

double distance(Point p1, Point p2) {
    double length;
    length = sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y)); 
    return length;
}


int main(int argc, char *argv[]) {
    char *fileName = argv[1];
    FILE *data;
    data = fopen(fileName, "rb");

    int n;
    fread(&n, sizeof(int), 1, data);

    Point point[10005];
    int i;
    double sum = 0;
    for (i = 1; i <= n; i++) {
        fread(&point[i].x, sizeof(int), 1, data);
        fread(&point[i].y, sizeof(int), 1, data);
        if (i == 1) {
            continue;
        }
        sum += distance(point[i], point[i-1]);
    }
    sum += distance(point[1], point[n]);
    printf("The perimeter is %.2lf\n", sum);
    return 0;
}  
