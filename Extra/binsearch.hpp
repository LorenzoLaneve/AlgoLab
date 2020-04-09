
#ifndef __bin_search
#define __bin_search

#include <vector>

long left_bin_search(long L, long R, const std::function<bool(long)> &predicate) {

    while (L < R) {
        long mid = (L + R)/2;
        
        if (predicate(mid)) {
            L = mid + 1;
        } else {
            R = mid;
        }
    }

    return L;
}

long right_bin_search(long L, long R, const std::function<bool(long)> &predicate) {

    while (L < R) {
        long mid = (L + R)/2;
        
        if (predicate(mid)) {
            L = mid + 1;
        } else {
            R = mid;
        }
    }

    return L - 1;
}


#endif