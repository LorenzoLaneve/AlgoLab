//
//  Corbusier
//  AlgoLab
//
//  Copyright © 2019 Lorenzo Laneve. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <vector>

typedef enum {
    FALSE = 0,
    TRUE = 1,
    NEW = 2
} boolp;

#define memo(p, h, nonempty) ((*_memo)[2*(heights.size()*(h) + (p)) + (nonempty)])

std::vector<boolp> *_memo;

// p is the position of the number we are considering
// h is the current height
bool check_heights(long p, long h, bool nonempty, long i, long k, const std::vector<long> &heights) {
    h %= k;
    if (h == i && nonempty) return true;
    if (p >= heights.size()) return false;

    if (memo(p, h, nonempty) != NEW) return memo(p, h, nonempty); 

    // take or pass
    return memo(p, h, nonempty) = (boolp)(check_heights(p+1, h + heights[p], true, i, k, heights) || check_heights(p+1, h, nonempty, i, k, heights));
}

int main(int argc, const char * argv[]) {
    
    std::istream &fp = std::cin;
    //std::fstream fp("/Users/lore/Desktop/AlgoLab/AlgoLab/Week 13/week13/corbusier/test3.in");
    
    int testcases;
    fp >> testcases;
    
    for (int t = 0; t < testcases; t++) {
        long n, i, k;
        fp >> n >> i >> k;

        std::vector<long> heights(n);
        for (long i = 0; i < n; i++) {
            long h; fp >> h;
            heights[i] = h % k;
        }

        std::vector<boolp> m(2*n*k, NEW);
        _memo = &m;

        std::cout << (check_heights(0, 0, false, i, k, heights) ? "yes\n" : "no\n");  

        _memo = nullptr;
    }
    
    return 0;
}
