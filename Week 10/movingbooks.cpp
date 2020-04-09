//
//  Moving Books
//  AlgoLab
//
//  Copyright © 2019 Lorenzo Laneve. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <queue>
#include <list>
#include <vector>
#include <algorithm>

int main(int argc, const char * argv[]) {
    
    std::istream &fp = std::cin;
    //std::fstream fp("/Users/lore/Desktop/AlgoLab/AlgoLab/Week 10/week10/moving_books/test2.in");
    
    int testcases;
    fp >> testcases;
    
    for (int t = 0; t < testcases; t++) {
        int n, m;
        fp >> n >> m;

        std::vector<long> s_(n);
        for (int i = 0; i < n; i++) {
            fp >> s_[i];
        } 

        std::vector<long> w_(m);
        for (int i = 0; i < m; i++) fp >> w_[i];

        std::sort(s_.begin(), s_.end());
        std::sort(w_.begin(), w_.end(), [&](const long i, const long j) { return i > j; });

        std::deque<long> s(s_.begin(), s_.end());
        std::list<long> w(w_.begin(), w_.end());

        if (s.back() < w.front()) { // if strongest cannot pick heaviest
            std::cout << "impossible\n";
            continue;
        }

        long total_time = -1;
        while (!w.empty()) {
            total_time += 1; // go upstairs
            
            // remove friends that cannot pick boxes
            while (s.front() < w.back()) s.pop_front();
            
            int fr = s.size() - 1;
            for (auto it = w.begin(); it != w.end() && fr >= 0;) {
                if (s[fr] >= *it) {
                    // if fr can pick up box i, then make them pick it up
                    it = w.erase(it);
                    fr--; // see next friend
                } else it++;
            }

            total_time += 2; // bring boxes to van
        }

        std::cout << std::max(0l, total_time) << "\n";
    }
    
    return 0;
}
