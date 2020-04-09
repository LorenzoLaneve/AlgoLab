//
//  Dominoes
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
        int n;
        fscanf(stdin, "%d", &n);
        
        int fallenblocks = 0;
        
        
        int numoftimes = 0;
        
        
        int i = 0, h_i;
        while (i <= fallenblocks && i < n) {
            fscanf(stdin, "%d", &h_i);
            numoftimes++;
            
            if (fallenblocks < i + h_i - 1)
                fallenblocks = i + h_i - 1;
            i++;
        }
        
        for (; i < n; i++) {
            fscanf(stdin, "%d", &h_i);
            numoftimes++;
        }
        
        fprintf(stdout, "%d\n", (fallenblocks+1 < n) ? fallenblocks+1 : n);
        
    }
    
    return 0;
}
