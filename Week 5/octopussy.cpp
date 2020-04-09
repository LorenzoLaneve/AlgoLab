//
//  Octopussy
//  AlgoLab
//
//  Copyright © 2019 Lorenzo Laneve. All rights reserved.
//

#include <iostream>
#include <vector>
#include <queue>


#define standing_bomb(i) (((i) - 1)/2)
#define left_underlying_bomb(i) (2*(i) + 1)
#define right_underlying_bomb(i) (2*(i) + 2)



int main(int argc, const char * argv[]) {
    
    FILE *fp = stdin;//fopen("/Users/lore/Desktop/AlgoLab/AlgoLab/Week 5/week5/octopussy/testsets/test4.in", "r");
    
    int testcases;
    fscanf(fp, "%d", &testcases);
    
    for (int t = 0; t < testcases; t++) {
        int n;
        fscanf(fp, "%d", &n);
        
        std::vector<long> timers(n);
        for (int i = 0; i < n; i++) {
            long t_i;
            fscanf(fp, "%ld", &t_i);
            
            timers[i] = t_i;
        }
        
        
        for (int i = 0; i < (n - 1)/2; i++) {
            if (timers[i] < timers[left_underlying_bomb(i)]) {
                timers[left_underlying_bomb(i)] = timers[i];
            }
            if (timers[i] < timers[right_underlying_bomb(i)]) {
                timers[right_underlying_bomb(i)] = timers[i];
            }
        }
        
        auto less = [&](const long i, const long j) { return timers[i] > timers[j]; };
        
        std::priority_queue<long, std::vector<long>, decltype(less)> deactivable_bombs(less);
        for (int i = (n - 1)/2; i < n; i++) {
            deactivable_bombs.push(i);
        }
        
        bool deactivated[n];
        for (int i = 0; i < n; i++) deactivated[i] = false;
        
        
        int time_passed = 0;
        bool exploded = false;
        while (!deactivable_bombs.empty()) {
            long first_bomb = deactivable_bombs.top();
            deactivable_bombs.pop();
            
            if (timers[first_bomb] <= time_passed) {
                exploded = true;
                break;
            }
            
            deactivated[first_bomb] = true;
            time_passed++;
            if (first_bomb > 0) {
                long sibling = first_bomb & 1 ? right_underlying_bomb(standing_bomb(first_bomb)) : left_underlying_bomb(standing_bomb(first_bomb));
                
                if (deactivated[sibling]) {
                    deactivable_bombs.push(standing_bomb(first_bomb));
                }
            }
        }
        
        printf("%s\n", exploded ? "no" : "yes");
    }
    
    return 0;
}
