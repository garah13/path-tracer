#ifndef BINARYSEARCHUTIL_H
#define BINARYSEARCHUTIL_H

#include "vector"
#include "utils/comparable.h"

//template<class Iter, class T>
//Iter binary_search(Iter begin, Iter end, T val) {

//    Iter i = std::lower_bound(begin, end, val);
//    if (i != end && !(val < *i)) {
//        return i;
//    }
//}

template<class Iter, class T>
int binary_search(std::vector<Iter> &vec, int leftIndex, int rightIndex, T val, Comparable cmp) {
    if (leftIndex >= rightIndex) {
        return rightIndex;
    }
    int midpoint = (leftIndex + rightIndex) / 2;
    if (cmp.equal(vec[midpoint], val)) {
        return midpoint;
    } else if (cmp.lessThan(vec[midpoint], val)) {
        return binary_search(vec, midpoint + 1, rightIndex, val, cmp);
    } else {
        return binary_search(vec, leftIndex, midpoint - 1, val, cmp);
    }
}

#endif // BINARYSEARCHUTIL_H
