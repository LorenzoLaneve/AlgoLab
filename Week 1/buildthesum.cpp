//
//  Build the Sum
//  AlgoLab
//
//  Copyright © 2019 Lorenzo Laneve. All rights reserved.
//

#include <iostream>

int main(int argc, const char * argv[]) {
    
    int sums;
    fscanf(stdin, "%d", &sums);
    
    for (int i = 0; i < sums; i++) {
        int n;
        fscanf(stdin, "%d", &n);
        
        int a, s;
        s = 0;
        for (int j = 0; j < n; j++) {
            fscanf(stdin, "%d", &a);
            s += a;
        }
        printf("%d\n", s);
    }

    return 0;
}
