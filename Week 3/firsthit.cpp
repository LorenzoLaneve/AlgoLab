//
//  First Hit
//  AlgoLab
//
//  Copyright © 2019 Lorenzo Laneve. All rights reserved.
//

#include <iostream>
#include <algorithm>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/distance_predicates_2.h>


typedef CGAL::Exact_predicates_exact_constructions_kernel EPEC;


long floor_to_int(const EPEC::FT& x) {
    double a = std::floor(CGAL::to_double(x));
    while (a > x) a -= 1;
    while (a+1 <= x) a += 1;
    return (long) a;
}

template <typename T> boost::optional<EPEC::Point_2> intersect(const T &s1, const EPEC::Segment_2 &s2) {
    auto isect = CGAL::intersection(s1, s2);
    if (isect) {
        if (const EPEC::Point_2 *int_point = boost::get<EPEC::Point_2>(&*isect)) {
            return boost::optional<EPEC::Point_2>(*int_point);
        } else if (const EPEC::Segment_2 *int_segment = boost::get<EPEC::Segment_2>(&*isect)) {
            return boost::optional<EPEC::Point_2>(int_segment->start());
        }
        assert(0);
    }
    return boost::none;
}


int main(int argc, const char *argv[]) {
    
    FILE *fp = stdin;//fopen("/Users/lore/Desktop/AlgoLab/AlgoLab/Week 3/week3/firsthit/firsthit_test.in", "r");
    
    int n;
    do {
        fscanf(fp, "%d\n", &n);
        
        if (n) {
            long x, y, a, b;
            fscanf(fp, "%ld %ld %ld %ld\n", &x, &y, &a, &b);
            
            std::vector<EPEC::Segment_2> segments(n);
            for (int i = 0; i < n; i++) {
                long r, s, t, u;
                fscanf(fp, "%ld %ld %ld %ld\n", &r, &s, &t, &u);
                
                segments[i] = EPEC::Segment_2({ r, s }, { t, u });
            }
            
            std::random_shuffle(segments.begin(), segments.end());
            
            
            EPEC::Point_2 origin(x, y);
            EPEC::Point_2 pass(a, b);
            
            EPEC::Ray_2 ray(origin, pass);
            
            EPEC::Segment_2 finite_ray({ 0, 0 }, { 0, 0 });
            int i;
            bool to_infinity = true;
            for (i = 0; i < n; i++) {
                const EPEC::Segment_2 &s_i = segments[i];
                
                if (auto isect = intersect(ray, s_i)) {
                    finite_ray = EPEC::Segment_2(origin, *isect);
                    to_infinity = false;
                    i++;
                    break;
                }
            }
            
            if (to_infinity) {
                printf("no\n");
                continue;
            }
            
            for (; i < n; i++) {
                const EPEC::Segment_2 &s_i = segments[i];
                
                if (auto isect = intersect(finite_ray, s_i)) {
                    finite_ray = EPEC::Segment_2(origin, *isect);
                }
            }
            
            EPEC::Point_2 first_hit = finite_ray.end();
            printf("%ld %ld\n", floor_to_int(first_hit.x()), floor_to_int(first_hit.y()));
        }
    } while (n);
    
    return 0;
}
