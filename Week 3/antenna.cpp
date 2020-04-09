//
//  Antenna
//  AlgoLab
//
//  Copyright © 2019 Lorenzo Laneve. All rights reserved.
//

#include <iostream>

#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/distance_predicates_2.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>


typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt EPEC;


long ceil_to_int(const EPEC::FT& x) {
    double a = std::ceil(CGAL::to_double(x));
    while (a < x) a += 1;
    while (a-1 >= x) a -= 1;
    return (long) a;
}


int main(int argc, const char * argv[]) {
    
    FILE *fp = stdin;// fopen("/Users/lore/Desktop/AlgoLab/AlgoLab/Week 3/week3/antenna/antenna_test.in", "r");
    
    int n;
    do {
        fscanf(fp, "%d", &n);
        
        if (n) {
            EPEC::Point_2 pointset[n];
            for (int i = 0; i < n; i++) {
                long x, y;
                fscanf(fp, "%ld %ld", &x, &y);
                
                pointset[i] = { x, y };
            }
            
            CGAL::Min_circle_2<CGAL::Min_circle_2_traits_2<EPEC>> min_c(pointset, pointset + n, true);
            
            printf("%ld\n", ceil_to_int(CGAL::sqrt(min_c.circle().squared_radius())));
        }
    } while (n);
    
    return 0;
}
