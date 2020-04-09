//
//  Hit?
//  AlgoLab
//
//  Copyright © 2019 Lorenzo Laneve. All rights reserved.
//

#include <iostream>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>


typedef CGAL::Exact_predicates_inexact_constructions_kernel EPIC;


int main(int argc, const char * argv[]) {
    
    FILE *fp = stdin;//fopen("/Users/lore/Desktop/AlgoLab/AlgoLab/Week 3/week3/hit/hit_test.in", "r");
    
    int n;
    do {
        fscanf(fp, "%d", &n);
        
        if (n) {
            long x, y, a, b;
            fscanf(fp, "%ld %ld %ld %ld", &x, &y, &a, &b);
            
            
            EPIC::Point_2 origin(x, y);
            EPIC::Point_2 pass(a, b);
            
            EPIC::Ray_2 ray(origin, pass);
            
            int i;
            for (i = 0; i < n; i++) {
                long r, s, t, u;
                fscanf(fp, "%ld %ld %ld %ld", &r, &s, &t, &u);
                
                EPIC::Point_2 p1(r, s);
                EPIC::Point_2 p2(t, u);
                
                EPIC::Segment_2 s_i(p1, p2);
                
                if (CGAL::do_intersect(ray, s_i)) {
                    break;
                }
            }
            
            printf("%s\n", (i == n) ? "no" : "yes");
            
            for (i++; i < n; i++) {
                long r, s, t, u;
                fscanf(fp, "%ld %ld %ld %ld", &r, &s, &t, &u);
            }
        }
    } while (n);

    return 0;
}
