#pragma once

#include <assert.h>
#include <vector>
#include "inplaceMerge.h"

template<class RandIt, class Compare>
class RunsStack {
private:
    Compare comp;
    std::vector<std::pair<RandIt, ui32> > runs;
    RandIt last;

public:
    RunsStack(Compare _comp);

    void addRun(RandIt first, ui32 length);
    void mergeXY();
    void mergeYZ();

    size_t size() const;
    ui32 lenX() const;
    ui32 lenY() const;
    ui32 lenZ() const;
};

template<class RandIt, class Compare>
RunsStack<RandIt, Compare>::RunsStack(Compare _comp) :
        comp(_comp),
        runs(),
        last() {}
template<class RandIt, class Compare>
void RunsStack<RandIt, Compare>::addRun(RandIt first, ui32 length) {
    runs.emplace_back(first, length);
    last = first + length;
}
template<class RandIt, class Compare>
void RunsStack<RandIt, Compare>::mergeXY() {
    assert(runs.size() > 1);
    inplaceMerge(runs[runs.size() - 2].first,
                 runs[runs.size() - 1].first,
                 last,
                 comp);
    runs[runs.size() - 2].second += runs[runs.size() - 1].second;
    runs.pop_back();
}
template<class RandIt, class Compare>
void RunsStack<RandIt, Compare>::mergeYZ() {
    assert(runs.size() > 2);
    inplaceMerge(runs[runs.size() - 3].first,
                 runs[runs.size() - 2].first,
                 last,
                 comp);
    runs[runs.size() - 3].second += runs[runs.size() - 2].second;
    runs.erase(runs.end() - 2, runs.end() - 1);
}
template<class RandIt, class Compare>
size_t RunsStack<RandIt, Compare>::size() const {
    return runs.size();
}
template<class RandIt, class Compare>
ui32 RunsStack<RandIt, Compare>::lenX() const {
    assert(runs.size() > 0);
    return (runs.end() - 1)->second;
}
template<class RandIt, class Compare>
ui32 RunsStack<RandIt, Compare>::lenY() const {
    assert(runs.size() > 1);
    return (runs.end() - 2)->second;
}
template<class RandIt, class Compare>
ui32 RunsStack<RandIt, Compare>::lenZ() const {
    assert(runs.size() > 2);
    return (runs.end() - 3)->second;
}




enum EWhatMerge {
    WM_NoMerge,
    WM_MergeXY,
    WM_MergeYZ
};

class ITimSortParams {
public:
    virtual ui32 minRun(ui32 n) const = 0;
    virtual bool needMerge(ui32 lenX, ui32 lenY) const = 0;
    virtual EWhatMerge whatMerge(ui32 lenX, ui32 lenY, ui32 lenZ) const = 0;
    virtual ui32 getGallop() const = 0;
};

class defaultTimSortParams : public ITimSortParams {
public:
    ui32 minRun(ui32 n) const {
        ui32 flag = 0;
        while (n >= 64)
            flag |= n & 1, n >>= 1;
        return n + flag;
    }
    bool needMerge(ui32 lenX, ui32 lenY) const {
        return lenY <= lenX;
    }
    EWhatMerge whatMerge(ui32 lenX, ui32 lenY, ui32 lenZ) const {
        if (lenZ <= lenX + lenY)
            return lenX <= lenZ ? WM_MergeXY : WM_MergeYZ;
        return lenY <= lenX ? WM_MergeXY : WM_NoMerge;
    }
    ui32 getGallop() const {
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
void timSort(RandIt first, RandIt last, Compare comp,
             const ITimSortParams& params) {

    ui32 minrun = params.minRun(last - first);

    RunsStack<RandIt, Compare> runsStack(comp);
    for (RandIt current = first; current != last;) {
        if (current + 1 == last) {
            runsStack.addRun(current, 1);
            break;
        }
        RandIt runEnd = current + 2;
        while (runEnd != last &&
               (current + minrun > runEnd ||
                isSorted(runEnd - 2, runEnd - 1, runEnd, comp))) {
            ++runEnd;
        }
        ui32 runLength = runEnd - current;
        if (comp(*std::prev(runEnd), *current))
            for (ui32 i = 0; i < runLength / 2; ++i)
                std::iter_swap(current + i, runEnd - i - 1);
        insertionSort(current, runEnd);
        runsStack.addRun(current, runLength);
        current = runEnd;
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
    while (runsStack.size() > 1)
        runsStack.mergeXY();
}












