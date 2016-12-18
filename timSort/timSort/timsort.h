#pragma once

#include <assert.h>
#include <vector>
#include "inplaceMerge.h"
#include "runs.h"

class ITimSortParams {
public:
    virtual ui32 minRun(ui32 n) const = 0;
    virtual bool needMerge(ui32 lenX, ui32 lenY) const = 0;
    virtual EWhatMerge whatMerge(ui32 lenX, ui32 lenY, ui32 lenZ) const = 0;
    virtual ui32 getGallop() const = 0;
};

class defaultTimSortParams : public ITimSortParams {
public:
    virtual ui32 minRun(ui32 n) const {
        ui32 flag = 0;
        while (n >= 64)
            flag |= n & 1, n >>= 1;
        return n + flag;
    }
    virtual bool needMerge(ui32 lenX, ui32 lenY) const {
        return lenY <= lenX;
    }
    virtual EWhatMerge whatMerge(ui32 lenX, ui32 lenY, ui32 lenZ) const {
        if (lenZ <= lenX + lenY)
            return lenX <= lenZ ? WM_MergeXY : WM_MergeYZ;
        return lenY <= lenX ? WM_MergeXY : WM_NoMerge;
    }
    virtual ui32 getGallop() const {
        return 0;
    }
};

static const defaultTimSortParams DEFAULT;

template<class RandIt>
void timSort(RandIt first, RandIt last,
             const ITimSortParams& params = DEFAULT);
template<class RandIt, class Compare>
void timSort(RandIt first, RandIt last, Compare comp,
             const ITimSortParams& params = DEFAULT);

template<class RandIt>
void timSort(RandIt first, RandIt last,
             const ITimSortParams& params) {
    timSort(first, last,
            std::less<typename std::iterator_traits<RandIt>::value_type>(),
            params);
}

template<class RandIt, class Compare>
bool isSorted(RandIt first, RandIt second, RandIt third, Compare comp) {
    return !(comp(*first, *second) && comp(*third, *second) ||
             comp(*second, *first) && comp(*second, *third));
}


template<class RandIt, class Compare>
ui32 newRun(RandIt current, RandIt last,
            ui32 minrun,
            Compare comp) {
    RandIt runEnd = current + 2;
    while (runEnd != last &&
           (std::distance(current, runEnd) < static_cast<int>(minrun) ||
            isSorted(runEnd - 2, runEnd - 1, runEnd, comp))) {
        ++runEnd;
    }
    ui32 runLength = runEnd - current;
    if (comp(*std::prev(runEnd), *current))
        for (ui32 i = 0; i < runLength / 2; ++i)
            std::iter_swap(current + i, runEnd - i - 1);
    insertionSort(current, runEnd, comp);
    return runLength;
}

template<class RStack>
void contractStack(RStack& runsStack, const ITimSortParams& params) {
    while (runsStack.size() > 2) {
        bool stop = false;
        switch (params.whatMerge(runsStack.lenX(),
                                 runsStack.lenY(),
                                 runsStack.lenZ())) {
        case WM_MergeXY:
            runsStack.mergeXY();
            break;
        case WM_MergeYZ:
            runsStack.mergeYZ();
            break;
        case WM_NoMerge:
            stop = true;
            break;
        }
        if (stop)
            break;
    }
    if (runsStack.size() == 2 && params.needMerge(runsStack.lenX(),
                                                  runsStack.lenY()))
        runsStack.mergeXY();
}

template<class RandIt, class Compare>
void timSort(RandIt first, RandIt last, Compare comp,
             const ITimSortParams& params) {

    ui32 minrun = params.minRun(last - first);

    RunsStack<RandIt, Compare> runsStack(comp);
    for (RandIt current = first; current != last;) {
        if (current + 1 == last) {
            runsStack.addRun(current, 1);
            break;
        }
        ui32 runLength = newRun(current, last, minrun, comp);
        runsStack.addRun(current, runLength);
        current += runLength;
        contractStack(runsStack, params);
    }
    while (runsStack.size() > 1)
        runsStack.mergeXY();
}