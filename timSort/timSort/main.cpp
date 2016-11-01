#include <stdio.h>
#include <vector>
#include "inplaceMerge.h"

int main() {
    std::vector<int> a = { 1, 5, 6, 8, 9, 2, 3, 4, 7, 10 };
    inplaceMerge(a.begin(), a.begin() + 5, a.end());
    for (int i = 0; i < 10; ++i)
        printf("%d ", a[i]);
    system("pause");
}