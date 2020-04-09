//
//  Defensive Line
//  AlgoLab
//
//  Copyright © 2019 Lorenzo Laneve. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#define FAIL -2

#define len(iv) ((iv).second - (iv).first + 1)

#define tab(l, i) (_tab[(i)*(n+1) + (l)]) 


// l is the index of the leftmost defensor to consider
// i is the number of intervals to pick
long covered_defensors(long m, long n, const std::vector<long> &skip) {
    std::vector<long> _tab((m+1)*(n+1));

    for (int l = 0; l <= n; l++) tab(l, 0) = 0;
    for (int i = 1; i <= m; i++) tab(n, i) = FAIL;

    for (int i = 1; i <= m; i++) {
        for (int l = n - 1; l >= 0; l--) {
            long pass = tab(l+1, i);
            long take = FAIL;
            if (skip[l] >= 0) {
                take = tab(l+skip[l], i-1);
                if (take != FAIL) take += skip[l];
            }

            tab(l, i) = std::max(pass, take);
        }
    }

    return tab(0, m);
}


int main(int argc, const char * argv[]) {
    std::istream &fp = std::cin;
    //std::fstream fp("/Users/lore/Desktop/AlgoLab/AlgoLab/Week 10/week10/defensive_line/test5.in");
    
    int testcases;
    fp >> testcases;
    
    for (int t = 0; t < testcases; t++) {
        long n, m, k;
        fp >> n >> m >> k;

        std::vector<long> v(n+1);
        for (long i = 0; i < n; i++) fp >> v[i];
        v[n] = 0; // to simplify the sliding window


        std::vector<std::pair<long, long>> intervals;
        std::vector<long> skip(n, -1);

        long a, b, sum;
        a = b = 0;
        sum = v[0];
        while (b < n) {
            if (sum < k) sum += v[++b];
            else if (sum > k) {
                sum -= v[a++];
                
                if (a > b) {
                    b = a;
                    sum = v[a];
                }
            } else {
                intervals.push_back({ a, b });
                skip[a] = b - a + 1;
                sum -= v[a++];
                
                if (a > b) {
                    b = a;
                    sum = v[a];
                }
            }
        }

        if (intervals.size() < m) {
            std::cout << "fail\n";
            continue;
        }

        // for (auto &p : intervals) std::cout << "[" << p.first << "," << p.second << "] ";
        // std::cout << "\n";
        // for (auto &p : skip) std::cout << "[" << p << "] ";
        // std::cout << "\n";

        long result = covered_defensors(m, n, skip);
        if (result >= 0) std::cout << result << "\n";
        else std::cout << "fail\n";
    }
    
    return 0;
}