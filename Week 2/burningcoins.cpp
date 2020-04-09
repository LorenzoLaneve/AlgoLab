//
//  Burning Coins
//  AlgoLab
//
//  Copyright © 2019 Lorenzo Laneve. All rights reserved.
//

#include <iostream>
#include <algorithm>

template<typename T> class mat {
    
    T x;
    
    int n;
    T *m;
    
public:
    mat(int n) : n(n) {
        m = new T[n*n];
    }
    
    ~mat() {
        delete[] m;
    }
    
    T &at(int i, int j) {
        if (i < 0 || i >= n || j < 0 || j >= n) {
            x = 0;
            return x;
        }
        
        return m[i+n*j];
    }
    
};


static mat<int> *guarantees;

int guarantee(int *coins, int i, int j) {
    if (guarantees->at(i, j) == -1) {
        if (i == j) {
            return guarantees->at(i, j) = coins[i];
        }
        if (i > j) {
            return guarantees->at(i, j) = 0;
        }
        
        int L = coins[i] + std::min(guarantee(coins, i+2, j),   guarantee(coins, i+1, j-1));
        int R = coins[j] + std::min(guarantee(coins, i+1, j-1), guarantee(coins, i, j-2));
        
        guarantees->at(i, j) = std::max(L, R);
    }

    return guarantees->at(i, j);
}




int main(int argc, const char * argv[]) {
    
    FILE *fp = stdin;//fopen("/Users/lore/Desktop/AlgoLab/AlgoLab/Week 2/week2/burning_coins/test3.in", "r");
    
    int testcases;
    fscanf(fp, "%d", &testcases);
    
    for (int t = 0; t < testcases; t++) {
        int n;
        fscanf(fp, "%d", &n);
        
        guarantees = new mat<int>(n);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                guarantees->at(i, j) = -1;
            }
        }
        
        int coins[n];
        for (int i = 0; i < n; i++) {
            fscanf(fp, "%d", &coins[i]);
        }
        
        printf("%d\n", guarantee(coins, 0, n - 1));
        
        delete guarantees;
    }
    
    return 0;
}
