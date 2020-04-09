//
//  Motorcycles
//  AlgoLab
//
//  Copyright © 2019 Lorenzo Laneve. All rights reserved.
//

#include <iostream>
#include <algorithm>
#include <vector>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/distance_predicates_2.h>


#define truncate_ray(i) { \
    is_trunc[(i)] = true; \
    bikers_trunc_dirs[(i)] = EPEC::Segment_2(bikers_dirs[(i)].source(), *isect); \
}


typedef CGAL::Exact_predicates_exact_constructions_kernel EPEC;

inline EPEC::FT slope(const EPEC::Direction_2 &dir) {
    return dir.dy() / dir.dx();
}

int main(int argc, const char * argv[]) {
    
    FILE *fp = stdin;//fopen("/Users/lore/Desktop/AlgoLab/AlgoLab/Week 4/week4/motorcycles/test4.in", "r");
    
    int testcases;
    fscanf(fp, "%d", &testcases);
    
    for (int t = 0; t < testcases; t++) {
        long n;
        fscanf(fp, "%ld", &n);
        
        EPEC::Ray_2 bikers_dirs[n];
        
        long *k = new long[n];
        long *sources = new long[n];
        
        bool is_trunc[n];
        for (long i = 0; i < n; i++) {
            long y0, x1, y1;
            fscanf(fp, "%ld %ld %ld", &y0, &x1, &y1);
            
            EPEC::Point_2 source = { 0, y0 };
            EPEC::Point_2 dir_point = { x1, y1 };
            
            bikers_dirs[i] = EPEC::Ray_2(source, dir_point);
            
            is_trunc[i] = false;
            k[i] = i;
            sources[i] = y0;
        }
        
        std::sort(k, k + n, [&](long i, long j) { return sources[i] > sources[j]; });

        delete[] sources;
        
        std::vector<long> stack;
        
        stack.push_back(0);
        for (long i = 1; i < n; i++) {
            long last_ray_index = stack.back();

            EPEC::FT last_slope = slope(bikers_dirs[k[last_ray_index]].direction());
            EPEC::FT new_slope = slope(bikers_dirs[k[i]].direction());
            
            while (last_slope < new_slope) {
                // there's an intersection with the topmost ray in the stack.
                stack.pop_back();
                
                if (CGAL::abs(last_slope) < CGAL::abs(new_slope)) {
                    // the ray in the stack cuts the current
                    is_trunc[k[i]] = true;
                    stack.push_back(last_ray_index);
                    break;
                }
                
                // current ray cuts the ray in the stack.
                is_trunc[k[last_ray_index]] = true;
                
                if (stack.empty()) {
                    // the current ray cut all the previous rays
                    break;
                }

                last_ray_index = stack.back();
                last_slope = slope(bikers_dirs[k[last_ray_index]].direction());
            }
            
            
            if (!is_trunc[k[i]]) {
                stack.push_back(i);
            }
        }
        
        delete[] k;
        
        for (long i = 0; i < n; i++) {
            if (!is_trunc[i]) {
                printf("%ld ", i);
            }
        }
        printf("\n");

    }
    
    return 0;
}
