#include <stdio.h>
#include <vector>
#include "inplaceMerge.h"
#include "timsort.h"

int main() {
    int a[100];
    for (int i = 0; i < 100; ++i)
        a[i] = rand();
    timSort(a, a + 100);
    for (int i = 0; i < 100; ++i)
        printf("%d ", a[i]);
    system("pause");
}