#include <stdio.h>
#include <assert.h>
#include <vector>
#include <functional>
#include <algorithm>
#include <map>
#include <iostream>
#include <time.h>
#include <random>

#include "inplaceMerge.h"
#include "timsort.h"

/*
 *
 * GitHub link:
 * https://github.com/MrKaStep/TimSort/tree/dev/timSort/timSort
 *
 */

int *arr1, *arr2;

int main() {
    const int N = 10000;
    int a[N];
    for (ui32 i = 0; i < N; ++i)
        a[i] = rand();
    timSort(a, a + N);
    assert(std::is_sorted(a, a + N));
    system("pause");
    return 0;
}