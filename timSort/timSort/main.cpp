#include <stdio.h>
#include <assert.h>
#include <vector>
#include <functional>
#include <algorithm>
#include <map>
#include <iostream>
#include <time.h>

#include "timsort.h"

/*
 *
 * GitHub link:
 * https://github.com/MrKaStep/TimSort/tree/dev/timSort/timSort
 *
 */

int main() {
#pragma comment(linker, "/STACK:268435456")
    const int N = 1111;
    int a[N];
    for (ui32 i = 0; i < N; ++i)
        a[i] = rand();
    timSort(a, a + N);
    assert(std::is_sorted(a, a + N));
    return 0;
}