//
//  Planks
//  AlgoLab
//
//  Copyright © 2019 Lorenzo Laneve. All rights reserved.
//

#include <iostream>
#include <vector>
#include <cstring>
#include <cassert>
#include <algorithm>
#include <unordered_map>

#define four_factorial 24


#define h(k1_mask, k2_mask) ((k1_mask) << 20 | (k2_mask))

#define memo(k1_mask, k2_mask) ((*_memo)[h(k1_mask, k2_mask)])


std::unordered_map<long, long> *_memo;

long build_side(const std::vector<long> &k1, const std::vector<long> &k2, const std::vector<long> &k1_index,
                long side_length, int number_of_sides, long k1_ban = 0, long k2_ban = 0) {
    
    if (number_of_sides <= 1) {
        // just one side. it's necessarily 1
        return 1;
    }
    
    auto it = _memo->find(h(k1_ban, k2_ban));
    if (it != _memo->end()) {
        return it->second;
    }
    
    long number_of_combs = 0;

    const long n2 = k2.size();
    for (long s2 = 0; s2 < n2; s2++) { // if != 0, there is an already used plank in s2
        if (k2_ban & s2) {
            s2 += (k2_ban & s2) - 1; // shift all the bits in common with the ban mask (-1 for s2++)
            continue;
        }
        
        // leftmost binary search on k1
        long i = 0;
        long j = k1_index.size();
        while (i < j) {
            long mid = (i+j)/2;
            
            if (k1[k1_index[mid]] + k2[s2] < side_length) {
                i = mid + 1;
            } else {
                j = mid;
            }
        }
        
        while (i < k1_index.size() && k1[k1_index[i]] + k2[s2] == side_length) {
            long s1 = k1_index[i];
            
            if (!(k1_ban & s1)) { // if != 0, there is an already used plank in s1
                long new_k1_ban = k1_ban | s1;
                long new_k2_ban = k2_ban | s2;
                // DP to avoid recalculating with same ban masks many times
                
                number_of_combs += build_side(k1, k2, k1_index, side_length, number_of_sides - 1, new_k1_ban, new_k2_ban);
            }
            
            i++;
        }
        
    }

    return memo(k1_ban, k2_ban) = number_of_combs;
}




int main(int argc, const char * argv[]) {
    
    FILE *fp = stdin;//fopen("/Users/lore/Desktop/AlgoLab/AlgoLab/Week 5/week5/planks/testsets/test3.in", "r");
    
    int testcases;
    fscanf(fp, "%d", &testcases);
    
    for (int t = 0; t < testcases; t++) {
        long n;
        fscanf(fp, "%ld", &n);
        
        long l[n];
        for (int i = 0; i < n; i++) {
            long l_i;
            fscanf(fp, "%ld", &l_i);
            
            l[i] = l_i;
        }
        
        long sum = 0;
        for (int i = 0; i < n; i++) sum += l[i];
        
        if (sum % 4 != 0) {
            printf("0\n");
        } else {
            //std::random_shuffle(l, l+n);
            
            std::vector<long> p1(l, l+n/2);
            std::vector<long> p2(l+n/2, l+n);
            
            const long n1 = n/2;
            const long n2 = n - n/2;
            
            std::vector<long> k1(1 << n1);
            std::vector<long> k2(1 << n2);
            
            std::vector<long> k1_index(1 << n1);
            
            
            //
            // try to calculate k1 and k2 just once.
            // ...and randomize the content of p1 and p2 before doing it.
            // to still have a balanced partition in expectation
            //
            
            // calculate sum for every subset of the first n/2 planks.
            for (int s = 0; s < (1 << n1); s++) {
                k1[s] = 0;
                for (int i = 0; i < n1; i++) if (s & 1<<i) k1[s] += p1[i];
                
                k1_index[s] = s;
            }
            
            for (int s = 0; s < (1 << n2); s++) {
                k2[s] = 0;
                for (int i = 0; i < n1; i++) if (s & 1<<i) k2[s] += p2[i];
            }
            
            std::sort(k1_index.begin(), k1_index.end(), [&](const long i, const long j) { return k1[i] < k1[j]; });

            
            _memo = new std::unordered_map<long, long>();
            
            printf("%ld\n", build_side(k1, k2, k1_index, sum / 4, 4) / four_factorial);
            
            delete _memo;
        }
    }
    
    return 0;
}
