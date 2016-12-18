#pragma once
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
                 runs[runs.size() - 1].first,
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