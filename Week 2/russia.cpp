//
//  Russia
//  AlgoLab
//
//  Copyright © 2019 Lorenzo Laneve. All rights reserved.
//

#include <iostream>
#include <algorithm>

#define memo(i, j) (_memo[n*(i)+(j)])

int *_memo;

int max_possible_winning(int *x, int n, int p, int k, int m, int i, int j) {
    if (memo(i, j) >= 0) {
        return memo(i, j);
    }
    
    if (k < p) {
        if (j - i + 1 <= m + k - p) {
            return memo(i, j) = 0;
        }
    } else {
        if (j - i + 1 <= k - p) {
            return memo(i, j) = 0;
        }
    }
    
    if (i == j) {
        if (p == k) {
            return memo(i, j) = x[i];
        } else {
            return memo(i, j) = 0;
        }
    }
    // i < j
    
    int if_pick_left = max_possible_winning(x, n, (p+1) % m, k, m, i+1, j);
    int if_pick_right = max_possible_winning(x, n, (p+1) % m, k, m, i, j-1);
    
    if (p == k) { // count the coin
        return memo(i, j) = std::max(x[i] + if_pick_left, x[j] + if_pick_right);
    } else { // the coin is not yours
        return memo(i, j) = std::min(if_pick_left, if_pick_right);
    }
}



int main(int argc, const char *argv[]) {
    
    FILE *fp = fopen("/Users/lore/Desktop/AlgoLab/AlgoLab/Week 2/russia/test3.in", "r");
    
    int testcases;
    fscanf(fp, "%d", &testcases);
    
    for (int t = 0; t < testcases; t++) {
        int n, m, k;
        fscanf(fp, "%d %d %d", &n, &m, &k);
        
        int x[n];
        int sum = 0;
        for (int i = 0; i < n; i++) {
            int x_i;
            fscanf(fp, "%d", &x_i);
            
            x[i] = x_i;
            sum += x_i;
        }
        
        _memo = new int[n*n];
        for (int j = 0; j < n; j++) {
            for (int i = 0; i <= j; i++) {
                memo(i, j) = -1;
            }
        }
        
        printf("%d\n", max_possible_winning(x, n, 0, k, m, 0, n-1));
        delete[] _memo;
    }
    
    return 0;
}
