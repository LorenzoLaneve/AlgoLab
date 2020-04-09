//
//  Even Matrices
//  AlgoLab
//
//  Copyright © 2019 Lorenzo Laneve. All rights reserved.
//

#include <iostream>
#include <cstdlib>

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

int main(int argc, const char * argv[]) {
    
    int testcases;
    fscanf(stdin, "%d", &testcases);
    
    for (int t = 0; t < testcases; t++) {
        int n;
        fscanf(stdin, "%d", &n);
        
        mat<char> m_init(n);
        mat<char> m(n);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                int v;
                fscanf(stdin, "%d", &v);
                m.at(i, j) = m_init.at(i, j) = v;
            }
        }
        
        long sum_of_evens = 0;
        for (int d = n; d > 0;) {
            // count even pairs for each row
            for (int i = 0; i < d; i++) {
                int evens = 0;
                int odds = 0;
                
                for (int j = 0; j < n; j++) {
                    if (m.at(i, j) == 0) {
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
            }
            
            
            //
            d--;
            int inc = n - d;
            for (int i = 0; i < d; i++) {
                for (int j = 0; j < n; j++) {
                    m.at(i, j) ^= m_init.at(i+inc, j);
                    m.at(i, j) &= 1;
                }
            }
        }
        
        fprintf(stdout, "%ld\n", sum_of_evens);
    }
    
    
    
    return 0;
}
