//
//  Knapsack (Extra)
//  AlgoLab
//
//  Copyright © 2019 Lorenzo Laneve. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>

#define tab(i, s) (_tab[(n+1)*(s) + (i)])

#define EXTRACT_ITEMS

int main (int argc, const char *argv[]) {

    std::cin.sync_with_stdio(false);
    //std::istream &fp = std::cin;
    std::ifstream fp("/Users/lore/Desktop/AlgoLab/AlgoLab/Extra/tests/knapsack.in");

    int testcases;
    fp >> testcases;

    for (int t = 0; t < testcases; t++) {
        long n, B;
        fp >> n >> B;

        std::vector<long> profit(n);
        std::vector<long> size(n);

        for (long i = 0; i < n; i++) fp >> profit[i] >> size[i];

        std::vector<long> _tab((n+1)*(B+1));
        for (long s = 0; s <= B; s++) tab(n, s) = 0;

        for (long i = n - 1; i >= 0; i--) {
            for (long s = 0; s <= B; s++) {
                long max_profit = tab(i+1, s);
                if (s + size[i] <= B) {
                    // we can take i

                    long if_take = profit[i] + tab(i+1, s + size[i]);
                    max_profit = std::max(max_profit, if_take);
                }
                tab(i, s) = max_profit;
            }
        }

        std::cout << tab(0, 0) << "\n";

#ifdef EXTRACT_ITEMS
        long i = 0, s = 0;
        while (i < n) {

        }
#endif
    }
    return 0;
}