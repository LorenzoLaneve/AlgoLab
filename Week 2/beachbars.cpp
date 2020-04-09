//
//  Beach Bars
//  AlgoLab
//
//  Copyright © 2019 Lorenzo Laneve. All rights reserved.
//

#include <iostream>
#include <vector>
#include <cstring>

#define s(i) (_s[(i)+1000101])

int main(int argc, const char * argv[]) {
    
    FILE *fp = stdin;//fopen("/Users/lore/Desktop/AlgoLab/AlgoLab/Week 2/week2/beach_bars/test1.in", "r");
    
    int testcases;
    fscanf(fp, "%d", &testcases);
    
    for (int t = 0; t < testcases; t++) {
        int n;
        fscanf(fp, "%d", &n);
        
        unsigned char _s[2000202];
        memset(_s, 0, sizeof(unsigned char) * 2000202);
        
        int x_min = +1000000;
        int x_max = -1000000;
        for (int i = 0; i < n; i++) {
            int x_i;
            fscanf(fp, "%d", &x_i);
            s(x_i) = 1;
            
            if (x_i < x_min) x_min = x_i;
            if (x_i > x_max) x_max = x_i;
        }
        
        
        int sum = 0;
        for (int i = 0; i < 100; i++) {
            sum += s(x_min + i);
        }
        
        
        std::vector<int> candidate_places;
        
        int max = 0;
        for (int x = x_min; x <= x_max; x++) {
            sum -= s(x - 101);
            sum += s(x + 100);
            
            if (max < sum) {
                candidate_places.clear();
                max = sum;
            }
            
            if (max <= sum) {
                candidate_places.push_back(x);
            }
        }
        
        
        int min_distance = 100;
        std::vector<int> minimal_places;
        for (int x : candidate_places) {
            
            int distance;
            for (distance = 100; distance >= 0; distance--) {
                if (s(x + distance) != 0 || s(x - distance) != 0) break;
            }
            
            if (min_distance > distance) {
                minimal_places.clear();
                min_distance = distance;
            }
            
            if (min_distance >= distance) {
                minimal_places.push_back(x);
            }
        }
        

        printf("%d %d\n", max, min_distance);
        for (int x : minimal_places) {
            printf("%d ", x);
        }
        printf("\n");
    }
    
    return 0;
}
