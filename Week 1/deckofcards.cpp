//
//  <probname>
//  AlgoLab
//
//  Copyright © 2019 Lorenzo Laneve. All rights reserved.
//

#include <iostream>
#include <math.h>


int main(int argc, const char * argv[]) {
    
    int testcases;
    fscanf(stdin, "%d", &testcases);
    
    for (int t = 0; t < testcases; t++) {
        int n, k;
        fscanf(stdin, "%d %d", &n, &k);
        
        int v[n];
        for (int i = 0; i < n; i++) {
            fscanf(stdin, "%u", &v[i]);
        }
        
        int i = 0;
        int j = 0;
        
        int i_min = 0;
        int j_min = 0;
        long sum_min = v[0];
        
        long sum = v[0];
        
        while (j < n) {
            if (sum < k) {
                if (++j < n) {
                    sum += v[j];
                    
                    if (abs(sum - k) < abs(sum_min - k)) {
                        sum_min = sum;
                        i_min = i;
                        j_min = j;
                    }
                }
            } else if (sum > k) {
                if (i == j) {
                    if (++j < n) {
                        sum += v[j];
                        
                        // check should not be necessary as it always evaluates to false
                    } else {
                        break;
                    }
                }
                
                if (i++ <= j) {
                    sum -= v[i-1];
                    
                    if (abs(sum - k) < abs(sum_min - k)) {
                        sum_min = sum;
                        i_min = i;
                        j_min = j;
                    }
                }
            } else {
                if (abs(sum - k) < abs(sum_min - k)) {
                    sum_min = sum;
                    i_min = i;
                    j_min = j;
                }
                break;
            }
        }

        printf("%d %d\n", i_min, j_min);
    }
    
    return 0;
}
