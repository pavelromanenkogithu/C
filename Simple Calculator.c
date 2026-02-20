#include <stdio.h>

typedef int (*operation)(int, int);

int add(int a, int b) {
    return a + b;
}

int sub(int a, int b) {
    return a - b;
}

int mul(int a, int b) {
    return a * b;
}

int power(int a, int b) {
    int res = 1;
    for (int i = 0; i < b; i++) {
        res *= a;
    }
    return res;
}

int divide(int a, int b) {
    if (b == 0) {
        printf("Cannot divide by zero\n");
        return 0;
    }
    return a / b;
}

int main() {
    operation op[] = { add, sub, mul, power, divide };

    int opt, a, b;

    printf("Choose an operation:\n");
    printf("0. Add\n1. Sub\n2. Mul\n3. Power\n4. Div\n");

    scanf("%d", &opt);

    if (opt < 0 || opt > 4) {
        printf("Invalid option\n");
        return 1;
    }

    printf("Enter two numbers:\n");
    scanf("%d %d", &a, &b);

    int result = op[opt](a, b);
    printf("Result: %d\n", result);

    return 0;
}
