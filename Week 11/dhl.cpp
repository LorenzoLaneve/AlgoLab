//
//  DHL
//  AlgoLab
//
//  Copyright © 2019 Lorenzo Laneve. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <limits>

#define infinity (std::numeric_limits<long>::max())
#define FAIL (100000)

#define tab(na, nb) (_tab[(a.size()+1)*(nb)+(na)])

long take_all(const std::vector<long> &a, const std::vector<long> &b) {
    std::vector<long> _tab((a.size()+1)*(b.size()+1), -1);

    tab(0, 0) = 0;
    for (long na = 1; na <= a.size(); na++) tab(na, 0) = FAIL;
    for (long nb = 1; nb <= b.size(); nb++) tab(0, nb) = FAIL;

    for (long na = 1; na <= a.size(); na++) for (long nb = 1; nb <= b.size(); nb++) {
        long min_cost = tab(na - 1, nb - 1);
        min_cost = std::min(min_cost, tab(na, nb - 1));
        min_cost = std::min(min_cost, tab(na - 1, nb));

        tab(na, nb) = min_cost + (a[na - 1] - 1) * (b[nb - 1] - 1);
    }

    return tab(a.size(), b.size());
}

int main(int argc, const char * argv[]) {
    
    std::istream &fp = std::cin;
    //std::fstream fp("/Users/lore/Desktop/AlgoLab/AlgoLab/Week 11/week11/DHL/sample.in");
    
    int testcases;
    fp >> testcases;
    
    for (int t = 0; t < testcases; t++) {
        long n;
        fp >> n;
        
        std::vector<long> a(n);
        std::vector<long> b(n);
        for (int i = 0; i < n; i++) fp >> a[i];
        for (int i = 0; i < n; i++) fp >> b[i];

        long min_cost = take_all(a, b);
        std::cout << min_cost << "\n";
    }
    
    return 0;
}
