//
//  Boats
//  AlgoLab
//
//  Copyright © 2019 Lorenzo Laneve. All rights reserved.
//

#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>


struct boat {
    long ring_pos;
    long length;
};

int main(int argc, const char * argv[]) {
    
    FILE *fp = stdin;//fopen("/Users/lore/Desktop/AlgoLab/AlgoLab/Week 5/week5/boats/testsets/test2.in", "r");
    
    int testcases;
    fscanf(fp, "%d", &testcases);
    
    for (int t = 0; t < testcases; t++) {
        long n;
        fscanf(fp, "%ld", &n);
        
        std::vector<boat> boats(n);
        for (int i = 0; i < n; i++) {
            long l_i, p_i;
            fscanf(fp, "%ld %ld", &l_i, &p_i);
            
            boats[i] = { p_i, l_i };
        }
        
        std::sort(boats.begin(), boats.end(), [](const boat &i, const boat &j) { return i.ring_pos < j.ring_pos; });
        
        //for (int i = 1; i < n; i++) {
        //    assert(boats[i-1].ring_pos <= boats[i].ring_pos);
        //}
        
        
        
        
        long first_free_point = boats[0].ring_pos;

        long placed_boats = 1;
        for (int i = 1; i < n; i++) {

            if (first_free_point <= boats[i].ring_pos) {
                long min_finish_time = std::max(first_free_point + boats[i].length, boats[i].ring_pos);
                
                for (int j = i+1; j < n && boats[j].ring_pos <= min_finish_time; j++) {
                    long candidate_finish_time = std::max(first_free_point + boats[j].length, boats[j].ring_pos);
                    
                    if (candidate_finish_time < min_finish_time) {
                        min_finish_time = candidate_finish_time;
                        i = j;
                    }
                }
                    
                
                placed_boats++;
                    
                first_free_point = min_finish_time;
            }
            
        }
        
        printf("%ld\n", placed_boats);
        
    }
    
    return 0;
}
