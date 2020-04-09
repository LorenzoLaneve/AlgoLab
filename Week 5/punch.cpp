//
//  Punch
//  AlgoLab
//
//  Copyright © 2019 Lorenzo Laneve. All rights reserved.
//

#include <iostream>
#include <vector>
#include <limits>


#define infinity (std::numeric_limits<long>::max() - 500000)

struct result {
    long cost;
    long distinct_bevs;
};

result *_memo;

#define memo(a, b, c) (_memo[a + b*(n+1) + c*(n+1)*(k+1)])

// i is the beverage we are considering
// v is the volume of the beverages we have already taken
// is_new is whether i was never added
result take_bev(long i, long v, bool is_new, long n, long k, const std::vector<long> &_v, const std::vector<long> &_c) {

    if (v >= k) return { 0, 0 };

    if (i > n) return { infinity, 0 };

    result memoized = memo(i, v, is_new);
    if (memoized.cost >= 0) {
        return memoized;
    }
    

    result next = take_bev(i+1, v, true,        n, k, _v, _c); // if i do not pick a new i
    result same = take_bev(i, v + _v[i], false, n, k, _v, _c); // if i pick i

    same.cost += _c[i];
    if (is_new) same.distinct_bevs += 1;

    result ret;
    if (next.cost < same.cost) {
        ret = next;
    } else if (next.cost > same.cost) {
        ret = same;
    } else {
        ret = next.distinct_bevs > same.distinct_bevs ? next : same;
    }

    return memo(i, v, is_new) = ret; 
}


int main(int argc, const char * argv[]) {
    
    FILE *fp = stdin;//fopen("/Users/lore/Desktop/AlgoLab/AlgoLab/Week 5/week5/punch/testsets/test2.in", "r");
    
    int testcases;
    fscanf(fp, "%d", &testcases);
    
    for (int t = 0; t < testcases; t++) {
        int n, k;
        fscanf(fp, "%d %d", &n, &k);
        
        std::vector<long> c(n+1);
        std::vector<long> v(n+1);
        
        for (int i = 1; i <= n; i++) {
            fscanf(fp, "%ld %ld", &c[i], &v[i]);
        }
        // at least one liter of punch for everyone: k

        c[0] = infinity;
        v[0] = 0;
        
        _memo = new result[(n+1)*(k+1)*(2)]; // considered beverage x volume x whether beverage is new or not
        for (long i = 0; i < n+1; i++) {
            for (long v = 0; v < k+1; v++) {
                memo(i, v, false) = { -1, -1 };
                memo(i, v, true) = { -1, -1 };
            }
        }


        result r = take_bev(1, 0, true, n, k, v, c);

        std::cout << r.cost << " " << r.distinct_bevs << "\n";

        delete[] _memo;
    }
    
    return 0;
}
