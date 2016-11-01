#include <stdio.h>
#include <vector>
#include "inplaceMerge.h"

int main() {
    std::vector<int> a = { 9, 8, 4, 3, 1, 10, 7, 6, 5, 2 };
    selectionSort(a.begin(), a.end(), [](int a, int b) {return a > b; });
    for (int i = 0; i < 10; ++i)
        printf("%d ", a[i]);
    system("pause");
}