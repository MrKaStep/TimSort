#include <stdio.h>
#include <assert.h>
#include <vector>
#include <functional>
#include <algorithm>
#include <map>
#include <iostream>
#include <time.h>


/*
 *
 * GitHub link:
 * https://github.com/MrKaStep/TimSort/tree/dev/timSort/timSort
 *
 */


typedef unsigned int ui32;

template<class BidirIt>
void selectionSort(BidirIt first, BidirIt last);
template<class BidirIt, class Compare>
void selectionSort(BidirIt first, BidirIt last, Compare comp);

template<class BidirIt>
void insertionSort(BidirIt first, BidirIt last);
template<class BidirIt, class Compare>
void insertionSort(BidirIt first, BidirIt last, Compare comp);

template<class FwdIt>
void merge(FwdIt first, FwdIt middle, FwdIt last,
           FwdIt buffer);
template<class FwdIt, class Compare>
void merge(FwdIt first, FwdIt middle, FwdIt last,
           FwdIt buffer, Compare comp);

template<class FwdIt>
void swapSegments(FwdIt a, FwdIt b, ui32 length);

template<class RandIt>
void sortSegments(RandIt first, RandIt last, ui32 length);
template<class RandIt, class Compare>
void sortSegments(RandIt first, RandIt last, ui32 length,
                  Compare comp);

template<class RandIt>
void reorderSegments(RandIt first, RandIt middle, RandIt last, ui32 length);
template<class RandIt, class Compare>
void reorderSegments(RandIt first, RandIt middle, RandIt last, ui32 length,
                     Compare comp);

template<class RandIt>
void inplaceMerge(RandIt first, RandIt middle, RandIt last);
template<class RandIt, class Compare>
void inplaceMerge(RandIt first, RandIt middle, RandIt last,
                  Compare comp);

/*
 * Implementation
 */


template<class BidirIt>
void selectionSort(BidirIt first, BidirIt last) {
    selectionSort(first, last,
                  std::less<typename std::iterator_traits<BidirIt>::value_type>());
}
template<class BidirIt, class Compare>
void selectionSort(BidirIt first, BidirIt last, Compare comp) {
    for (; first != last; ++first) {
        BidirIt minimum = first;
        for (BidirIt it = first; it != last; ++it)
            if (comp(*it, *minimum))
                minimum = it;
        std::iter_swap(minimum, first);
    }
}

template<class BidirIt>
void insertionSort(BidirIt first, BidirIt last) {
    insertionSort(first, last,
                  std::less<typename std::iterator_traits<BidirIt>::value_type>());
}
template<class BidirIt, class Compare>
void insertionSort(BidirIt first, BidirIt last, Compare comp) {
    typedef typename std::iterator_traits<BidirIt>::value_type valueType;
    for (BidirIt toPush = std::next(first); toPush != last; ++toPush) {
        valueType val = *toPush;
        BidirIt cur = toPush;
        while (cur != first && comp(val, *std::prev(cur))) {
            *cur = *std::prev(cur);
            --cur;
        }
        *cur = val;
    }
}

template<class FwdIt>
void merge(FwdIt first, FwdIt middle, FwdIt last,
           FwdIt buffer) {
    merge(first, middle, last,
          std::less<typename std::iterator_traits<FwdIt>::value_type>());
}
template<class FwdIt, class Compare>
void merge(FwdIt first, FwdIt middle, FwdIt last,
           FwdIt buffer, Compare comp) {
    FwdIt left = first;
    for (FwdIt ptrBuff = buffer; left != middle; ++left, ++ptrBuff)
        std::iter_swap(left, ptrBuff);
    left = buffer;
    FwdIt right = middle;
    FwdIt write = first;
    FwdIt buffLast = buffer + (middle - first);
    while (left != buffLast && right != last)
        std::iter_swap(write++,
                       comp(*left, *right) ? left++ : right++);
    while (left != buffLast)
        std::iter_swap(write++, left++);
    while (right != last)
        std::iter_swap(write++, right++);
}

template<class FwdIt>
void swapSegments(FwdIt a, FwdIt b, ui32 length) {
    if (a == b)
        return;
    for (ui32 i = 0; i < length; ++i) {
        std::iter_swap(a++, b++);
    }
}

template<class RandIt>
void sortSegments(RandIt first, RandIt last, ui32 length) {
    sortSegments(first, last, length,
                 std::less<typename std::iterator_traits<RandIt>::value_type>());
}
template<class RandIt, class Compare>
void sortSegments(RandIt first, RandIt last, ui32 length, Compare comp) {
    for (; first != last; first += length) {
        RandIt minimum = first;
        for (RandIt it = first; it != last; it += length)
            if (comp(*it, *minimum) || !comp(*minimum, *it) &&
                comp(it[length - 1], minimum[length - 1]))
                minimum = it;
        swapSegments(minimum, first, length);
    }
}

template<class RandIt>
void reorderSegments(RandIt first, RandIt middle, RandIt last, ui32 length) {
    reorderSegments(first, middle, last, length,
                    std::less<typename std::iterator_traits<RandIt>::value_type>());
}
template<class RandIt, class Compare>
void reorderSegments(RandIt first, RandIt middle, RandIt last, ui32 length,
                     Compare comp) {
    ui32 segmentsCount = (last - first) / length;
    if (std::distance(first, middle) > static_cast<int>(length * segmentsCount))
        return;
    RandIt middleSegmentBegin = first + length * ((middle - first) / length);
    RandIt lastSegmentBegin = first + length * (segmentsCount - 1);
    swapSegments(middleSegmentBegin, lastSegmentBegin, length);
    sortSegments(first, lastSegmentBegin, length, comp);
}

template<class RandIt>
void inplaceMerge(RandIt first, RandIt middle, RandIt last) {
    inplaceMerge(first, middle, last,
                 std::less<typename std::iterator_traits<RandIt>::value_type>());
}
template<class RandIt, class Compare>
void inplaceMerge(RandIt first, RandIt middle, RandIt last, Compare comp) {
    if (last - first < 17) {
        selectionSort(first, last, comp);
        return;
    }
    ui32 totalLength = last - first;
    ui32 leftLength = middle - first;
    ui32 rightLength = last - middle;
    ui32 segmentLength = 0;
    while ((segmentLength + 1) * (segmentLength + 1) <= totalLength)
        ++segmentLength;
    ui32 segmentsCount = totalLength / segmentLength;

    if (std::distance(first, middle) < static_cast<int>(segmentLength * segmentsCount)) {
        reorderSegments(first, middle, last, segmentLength, comp);
        RandIt lastSegmentBegin = first + segmentLength * (segmentsCount - 1);
        for (ui32 i = 0; i + 2 < segmentsCount; ++i)
            merge(first + segmentLength * i,
                  first + segmentLength * (i + 1),
                  first + segmentLength * (i + 2),
                  lastSegmentBegin,
                  comp);

    }
    ui32 badSegmentLength = segmentLength + totalLength % segmentLength;
    RandIt badSegmentBegin = last - badSegmentLength;
    selectionSort(last - 2 * badSegmentLength, last, comp);
    RandIt toMerge = badSegmentBegin;
    while (toMerge != first) {
        RandIt mergeFirst = std::distance(first, toMerge) >= static_cast<int>(badSegmentLength) ?
            toMerge - badSegmentLength : first;
        merge(mergeFirst,
              toMerge,
              toMerge + badSegmentLength,
              badSegmentBegin,
              comp);
        toMerge = mergeFirst;
    }
    selectionSort(badSegmentBegin, last, comp);
    return;
}


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
               (std::distance(current, runEnd) < static_cast<int>(minrun) ||
                isSorted(runEnd - 2, runEnd - 1, runEnd, comp))) {
            ++runEnd;
        }
        ui32 runLength = runEnd - current;
        if (comp(*std::prev(runEnd), *current))
            for (ui32 i = 0; i < runLength / 2; ++i)
                std::iter_swap(current + i, runEnd - i - 1);
        insertionSort(current, runEnd, comp);
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

int *arr;

bool cmp(const int &a, const int &b) {
    return arr[a] < arr[b];
}

class myParams : public defaultTimSortParams {
    ui32 minRun(ui32 n) const {
        return 1;
    }
};

int main() {
#pragma comment(linker, "/STACK:268435456")
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    int n, *ind, *buff;
    scanf("%d", &n);
    ind = new int[n];
    arr = new int[n];
    buff = new int[n];
    for (int i = 0; i < n; ++i) {
        scanf("%d", &arr[i]);
        ind[i] = i;
    }
    timSort(ind, ind + n, cmp);
    if (!(std::is_sorted(ind, ind + n, cmp))) {
        std::cerr << n << '\n';
        //std::cerr << "arr:\n";
        //for (int i = 0; i<n; ++i)
        //    std::cerr << arr[i] << ",\n";
        std::cerr << "ind:\n";
        for (int i = 0; i<n; ++i)
            std::cerr << ind[i] << ",\n";
        assert(std::is_sorted(ind, ind + n, cmp));
    }
    int l = 0, r = 0, ansl, ansr;
    long long s = 0, ans = 0;
    while (r < n) {
        while (r - l > 0 && arr[ind[r]] - arr[ind[l]] > arr[ind[l + 1]]) {
            s -= arr[ind[l]];
            ++l;
        }
        s += arr[ind[r]];
        ++r;
        if (s > ans) {
            ansl = l;
            ansr = r;
            ans = s;
        }
    }
    printf("%lld\n", ans);
    for (int i = ansl; i < ansr; ++i) {
        arr[i - ansl] = ind[i] + 1;
    }
    delete[] ind;
    timSort(arr, arr + ansr - ansl);
    assert(std::is_sorted(arr, arr + ansr - ansl));
    delete[] buff;
    for (int i = 0; i < ansr - ansl; ++i) {
        printf("%d ", arr[i]);
    }
    delete[] arr;
    //srand(time(NULL));
    //const int N = 17;
    //std::vector<int> v(N), u(N);
    //for (int i = 0; i < N; ++i)
    //    u[i] = (rand() << 16) | rand(),
    //    v[i] = i;
    //auto cmp = [&u](int a, int b) {return u[a] < u[b]; };
    //timSort(v.begin(), v.end(), cmp, myParams());
    //assert(std::is_sorted(v.begin(), v.end(), cmp));
    return 0;
}