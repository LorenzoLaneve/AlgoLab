//
//  Even Pairs
//  AlgoLab
//
//  Copyright © 2019 Lorenzo Laneve. All rights reserved.
//

#include <iostream>

int main(int argc, const char * argv[]) {
    
    int testcases;
    fscanf(stdin, "%d", &testcases);
    
    for (int t = 0; t < testcases; t++) {
        int n;
        fscanf(stdin, "%d", &n);
        
        int evens = 0;
        int odds = 0;
        long sum_of_evens = 0;
        
        for (int i = 0; i < n; i++) {
            int v_i;
            fscanf(stdin, "%d", &v_i);
            
            if (v_i == 0) {
                evens = 1 + evens;
                odds = 0 + odds;
                // the new '0' alone is 1 new even pair
                // the new '0' concatenated with the even pairs from the last step are {evens} new even pairs
                // the new '0' concatenated with the odd pairs from the last step are {odds} new odd pairs
            } else {
                int temp = evens;
                evens = 0 + odds;
                odds = 1 + temp;
                // the new '1' concatenated with the odd pairs from the last step are {odds} new even pairs
                // the new '1' alone is 1 new odd pair
                // the new '1' concatenated with the even pairs from the last step are {evens} new odd pairs
            }
            sum_of_evens += evens;
        }
        
        fprintf(stdout, "%ld\n", sum_of_evens);
    }
    
    return 0;
}
