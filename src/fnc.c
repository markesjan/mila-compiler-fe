#include <stdio.h>

int writeln(int x) {
    printf("%d\n", x);
    return 0;
}

int write(int x) {
    printf("%d", x);
    return 0;
}

int readln(int* x) {
    scanf("%d", x);
    return 0;
}

int writelnReal(double x) {
    printf("%lf\n", x);
    return 0;
}

int writeReal(double x) {
    printf("%lf", x);
    return 0;
}

int readlnReal(double* x) {
    scanf("%lf", x);
    return 0;
}

int writelnString(const char* x) {
    printf("%s\n", x);
    return 0;
}

int writeString(const char* x) {
    printf("%s", x);
    return 0;
}
