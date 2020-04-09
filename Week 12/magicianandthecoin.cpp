//
//  Magician and The Coin
//  AlgoLab
//
//  Copyright © 2019 Lorenzo Laneve. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>

typedef double probability;


std::vector<probability> *_memo;

#define memo(i, w) ((*_memo)[(n+1)*(w) + (i)])

// i is the number of remaining days
// w is the current wealth
probability max_probability(long i, long w, long m, long n, const std::vector<probability> &p) {
    if (w >= m) return 1.0; // stop betting! we are done 
    if (i <= 0) return 0.0; // fail

    if (memo(i, w) >= 0) return memo(i, w);
    
    probability p_i = p[n - i]; 

    probability max_p = 0.0;
    for (long b_i = 0; b_i <= w; b_i++) {
        probability total_p = 0.0;
        total_p +=      p_i  * max_probability(i - 1, w + b_i, m, n, p);
        total_p += (1 - p_i) * max_probability(i - 1, w - b_i, m, n, p);

        max_p = std::max(max_p, total_p);
    }

    return memo(i, w) = max_p;
}

int main(int argc, const char * argv[]) {
    
    std::istream &fp = std::cin;
    //std::fstream fp("/Users/lore/Desktop/AlgoLab/AlgoLab/Week 12/week12/magician_coin/test3.in");
    
    int testcases;
    fp >> testcases;
    
    for (int t = 0; t < testcases; t++) {
        long n, k, m;
        fp >> n >> k >> m;

        std::vector<probability> p(n);
        for (long i = 0; i < n; i++) fp >> p[i];


        std::vector<probability> mm((n+1)*m, -1);
        _memo = &mm;
        probability result = max_probability(n, k, m, n, p);

        printf("%.5lf\n", result);
        
        //std::cout << std::fixed << std::setprecision(5);
        //std::cout << result << std::endl;
    }
    
    return 0;
}
