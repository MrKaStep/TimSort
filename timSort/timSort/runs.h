#pragma once
#include "inplaceMerge.h"

template<class RandIt>
class Run {
    RandIt first;
    RandIt last;
    Run();
    Run(RandIt _first, RandIt _last);
    Run(RandIt _first, ui32 length);
};