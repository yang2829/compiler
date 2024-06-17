#include <stdio.h>
#include <stdlib.h>

int func();

int main() {
    int a = +3, b = 5;
    for (int c = 5; c > 0; c = c-1) {
        a = a + c;
    }
    if (a > 10)
        a = 10;
    else if (a < 0)
        a = 0;
    else
        b = a;
    int d = func();
    // printf("%d\n", 3 == 3 == 1);
    // system("pause");
    return 0
}

int func() {
    int fa = 5;
    while (fa < 100) {
        fa = fa * 2;
    }
    return fa;
}
