//
//  Longest Increasing Subsequence (Extra)
//  AlgoLab
//
//  Copyright © 2019 Lorenzo Laneve. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>

#define EXTRACT_SEQUENCE

int main (int argc, const char *argv[]) {

    std::cin.sync_with_stdio(false);
    //std::istream &fp = std::cin;
    std::ifstream fp("/Users/lore/Desktop/AlgoLab/AlgoLab/Extra/tests/lis.in");

    int testcases;
    fp >> testcases;

    for (int t = 0; t < testcases; t++) {
        long n;
        fp >> n;

        std::vector<long> F(n+1, -1); // index of final val for increasing subseq of length i
        std::vector<long> s(n); // input

#ifdef EXTRACT_SEQUENCE
        std::vector<long> P(n, -1); // prev element of i
#endif

        fp >> s[0];
        F[1] = 0;

        long lis_len = 1;
        for (long i = 1; i < n; i++) { // consider s[i]
            fp >> s[i];

            auto it = std::upper_bound(F.begin() + 1, F.begin() + lis_len + 1, i, [&](long a, long b) { return s[a] < s[b]; });
            long L = it - F.begin();

            // L is the leftmost element such that s[i] <= s[F[L]]
            // s[F[L - 1]] < s[i] <= s[F[L]] ===> s[i] can be attached to L - 1 sequence to form a better L sequence

#ifdef EXTRACT_SEQUENCE
            if (L > 1) P[i] = F[L - 1]; // i's prev value is L - 1 sequence's last element
#endif

            F[L] = i;
            lis_len = std::max(lis_len, L);
        }

        std::cout << lis_len << "\n";

#ifdef EXTRACT_SEQUENCE
        std::deque<long> lis_vec;
        int k = F[lis_len];
        while (k >= 0) {
            lis_vec.push_front(s[k]);
            k = P[k];
        }

        for (long e : lis_vec) std::cout << e << " "; 
        std::cout << "\n";
#endif
    }

    return 0;
}
