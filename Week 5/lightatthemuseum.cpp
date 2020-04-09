//
//  Light at The Museum
//  AlgoLab
//
//  Copyright © 2019 Lorenzo Laneve. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#define diff(sw, rm) (_diff[n*(rm) + (sw)])

// only on room 0
long config_diff_c1(long c1, long n1, long n2, const std::vector<long> &_diff) {
    long config_diff = 0;
    for (long sw = 0; sw < n1; sw++) if (c1 & 1<<sw) config_diff += _diff[sw];
    return config_diff;
}

// only on room 0
long config_diff_c2(long c2, long n1, long n2, const std::vector<long> &_diff) {
    long config_diff = 0;
    for (long sw = 0; sw < n2; sw++) if (c2 & 1<<sw) config_diff += _diff[sw + n1];
    return config_diff;
}

long config_diff(long room_id, long c1, long c2, long n1, long n2, const std::vector<long> &_diff) {
    const long n = n1 + n2;
    long config_diff = 0;
    for (long sw = 0; sw < n1; sw++) if (c1 & 1<<sw) config_diff += diff(sw, room_id);
    for (long sw = 0; sw < n2; sw++) if (c2 & 1<<sw) config_diff += diff(sw + n1, room_id);
    return config_diff;
}

long split_and_list(long m, long n, const std::vector<long> &on, const std::vector<long> &_diff) {
    const long n1 = n/2;
    const long n2 = n - n/2;

    std::vector<long> mem1(1 << n1);
    std::vector<long> k(1 << n1);

    for (long c1 = 0; c1 < (1 << n1); c1++) {
        mem1[c1] = config_diff_c1(c1, n1, n2, _diff);
        k[c1] = c1;
    }

    std::sort(k.begin(), k.end(), [&](const long i, const long j) { return mem1[i] < mem1[j]; });

    long min_lights = 1l << 30;
    for (long c2 = 0; c2 < (1 << n2); c2++) {
        const long target = on[0] + config_diff_c2(c2, n1, n2, _diff);

        long L = 0;
        long R = 1 << n1;
        while (L < R) {
            long mid = (L + R)/2;

            if (mem1[k[mid]] + target < 0) {
                L = mid + 1;
            } else {
                R = mid;
            }
        }

        // L should be the leftmost result
        while (L < (1 << n1) && mem1[k[L]] + target == 0) {
            const long c1 = k[L++];
            // c1.c2 satisfies room 0! check other rooms

            bool ok = true;
            for (long rm = 1; rm < m; rm++) {
                if (on[rm] + config_diff(rm, c1, c2, n1, n2, _diff) != 0) {
                    // this config is not what we are looking for
                    ok = false;
                    break;
                }
            }
            
            if (ok) {
                // update min bits
                long num_bits = 0;
                for (long sw = 0; sw < n1; sw++) if (c1 & 1<<sw) num_bits++;
                for (long sw = 0; sw < n2; sw++) if (c2 & 1<<sw) num_bits++;
                min_lights = std::min(min_lights, num_bits);
            }
        }
    }
    
    return min_lights;
}


int main(int argc, const char * argv[]) {
    
    std::cin.sync_with_stdio(false);
    std::istream &fp = std::cin;
    //std::fstream fp("/Users/lore/Desktop/AlgoLab/AlgoLab/Week 5/week5/light_at_the_museum/testsets/test3.in");
    
    int testcases;
    fp >> testcases;
    
    for (int t = 0; t < testcases; t++) {
        long n, m;
        fp >> n >> m;

        std::vector<long> on(m);
        for (long i = 0; i < m; i++) {
            long b_i; fp >> b_i;
            on[i] = -b_i;
        } 

        std::vector<long> _diff(n*m);
        for (long sw = 0; sw < n; sw++) {
            for (long rm = 0; rm < m; rm++) {
                long on_i, off_i; fp >> on_i >> off_i;
                on[rm] += on_i;
                diff(sw, rm) = off_i - on_i;
            }
        }

        // we want to find a configuration that cancels out every component of on
        // try to cancel the first component: for each configuration that cancels the first component check the others

        long result = split_and_list(m, n, on, _diff);
        if (result > n) {
            std::cout << "impossible\n";
        } else std::cout << result << "\n";
    }
    
    return 0;
}