#pragma once

#include <functional>

#ifndef UI32
#define UI32
typedef unsigned int ui32;
#endif ///UI32

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

template<class RandIt, class Compare>
void handleNormalSegments(RandIt first, RandIt middle, RandIt last,
                          ui32 segmentLength, ui32 segmentsCount,
                          Compare comp) {
    reorderSegments(first, middle, last, segmentLength, comp);
    RandIt lastSegmentBegin = first + segmentLength * (segmentsCount - 1);
    for (ui32 i = 0; i + 2 < segmentsCount; ++i)
        merge(first + segmentLength * i,
                first + segmentLength * (i + 1),
                first + segmentLength * (i + 2),
                lastSegmentBegin,
                comp);
}

template<class RandIt, class Compare>
void handleBadSegment(RandIt first, RandIt last,
                      ui32 segmentLength, ui32 totalLength,
                      Compare comp){
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
        handleNormalSegments(first, middle, last, segmentLength, segmentsCount, comp);
    }
    handleBadSegment(first, last, segmentLength, totalLength, comp);
    return;
}