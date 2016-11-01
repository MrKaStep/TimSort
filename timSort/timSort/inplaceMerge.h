#pragma once
#include <algorithm>
typedef unsigned int ui32;

template<class BidirIt>
void selectionSort(BidirIt first, BidirIt last);
template<class BidirIt, class Compare>
void selectionSort(BidirIt first, BidirIt last, Compare comp);

template<class FwdIt>
void merge(FwdIt first, FwdIt middle, FwdIt last,
           FwdIt buffer);
template<class FwdIt, class Compare>
void merge(FwdIt first, FwdIt middle, FwdIt last,
           FwdIt buffer, Compare comp);

template<class RandIt>
void inplaceMerge(RandIt first, RandIt middle, RandIt last);
template<class RandIt, class Compare>
void inplaceMerge(RandIt first, RandIt middle, RandIt last,
                  Compare comp);

template<class BidirIt>
void selectionSort(BidirIt first, BidirIt last) {
    selectionSort(first, last,
          std::less<std::iterator_traits<BidirIt>::value_type>());
}
template<class BidirIt, class Compare>
void selectionSort(BidirIt first, BidirIt last, Compare comp) {
    typedef std::iterator_traits<BidirIt>::value_type valueType;
    for (; first != last; ++first) {
        BidirIt minimum = first;
        for (BidirIt it = first; it != last; ++it)
            if (comp(*it, *minimum))
                minimum = it;
        std::iter_swap(minimum, first);
    }
}

template<class FwdIt>
void merge(FwdIt first, FwdIt middle, FwdIt last,
           FwdIt buffer) {
    merge(first, middle, last,
          std::less<std::iterator_traits<FwdIt>::value_type>());
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
    while (write != last && left != buffLast)
        std::iter_swap(write++,
                       comp(*left, *right) ? left++ : right++);
}

template<class FwdIt>
void swapSegments(FwdIt a, FwdIt b, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        std::iter_swap(a++, b++);
    }
}

template<class RandIt>
void inplaceMerge(RandIt first, RandIt middle, RandIt last) {
    inplaceMerge(first, middle, last,
                 std::less<std::iterator_traits<RandIt>::value_type>());
}
template<class RandIt, class Compare>
void inplaceMerge(RandIt first, RandIt middle, RandIt last, Compare comp) {
    typedef std::iterator_traits<RandIt>::value_type valueType;






    return;
}