//
//  The Great Game
//  AlgoLab
//
//  Copyright © 2019 Lorenzo Laneve. All rights reserved.
//

#include <iostream>
#include <algorithm>
#include <vector>

class memo {
    
    int n;
    int *_min;
    int *_max;
    
public:
    memo(int n) : n(n) {
        _min = new int[n];
        _max = new int[n];
        
        for (int i = 0; i < n; i++) {
            _min[i] = -1;
            _max[i] = -1;
        }
    }
    
    ~memo() {
        delete[] _min;
        delete[] _max;
    }
    
    int &min(int pos) {
        return _min[pos];
    }
    
    int &max(int pos) {
        return _max[pos];
    }
    
};

memo *_memo;

int turns_min(std::vector<int> *transitions, int n, int pos);

int turns_max(std::vector<int> *transitions, int n, int pos) {
    if (pos == n) {
        return 0;
    }
    
    if (_memo->max(pos) >= 0)
        return _memo->max(pos);
    
    int max_turns = -1;
    for (int new_pos : transitions[pos]) {
        int turns_after_transition = turns_min(transitions, n, new_pos);
        if (max_turns < turns_after_transition) max_turns = turns_after_transition;
    }
    
    return _memo->max(pos) = 1 + max_turns;
}

int turns_min(std::vector<int> *transitions, int n, int pos) {
    if (pos == n) {
        return 0;
    }
    
    if (_memo->min(pos) >= 0)
        return _memo->min(pos);
    
    int min_turns = 2147483647;
    for (int new_pos : transitions[pos]) {
        int turns_after_transition = turns_max(transitions, n, new_pos);
        if (min_turns > turns_after_transition) min_turns = turns_after_transition;
    }
    
    return _memo->min(pos) = 1 + min_turns;
}


int main(int argc, const char *argv[]) {
    
    FILE *fp = stdin;//fopen("/Users/lore/Desktop/AlgoLab/AlgoLab/Week 2/week2/great_game/sample.in", "r");
    
    int testcases;
    fscanf(fp, "%d", &testcases);
    
    for (int t = 0; t < testcases; t++) {
        int n, m;
        fscanf(fp, "%d %d", &n, &m);
        
        std::vector<int> transitions[n];
        
        int r, b;
        fscanf(fp, "%d %d", &r, &b);
        
        for (int i = 0; i < m; i++) {
            int u, v;
            fscanf(fp, "%d %d", &u, &v);
            
            transitions[u].push_back(v);
        }
        
        _memo = new memo(n);
        
        int red_turns = turns_min(transitions, n, r);
        int black_turns = turns_min(transitions, n, b);
        
        int k;
        for (k = 1; red_turns > 0 && black_turns > 0; k = (k+1) % 4) {
            if (k >> 1 == 0) {
                red_turns--;
            } else {
                black_turns--;
            }
        }
        
        //assert((red_turns == 0 && black_turns != 0) || (red_turns != 0 && black_turns == 0));
        
        printf("%d\n", !black_turns);
        delete _memo;
        
    }
    
    return 0;
}
