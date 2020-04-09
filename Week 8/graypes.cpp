//
//  Graypes
//  AlgoLab
//
//  Copyright © 2019 Lorenzo Laneve. All rights reserved.
//

#include <iostream>
#include <fstream>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Triangulation::Finite_faces_iterator Face_iterator;


typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt EPEC;

long ceil_to_int(EPEC::FT &x) {
    double a = CGAL::to_double(x);
    while (a < x) a += 1;
    while (a-1 >= x) a -= 1;
    return (long) a;
}

int main(int argc, const char * argv[]) {
    
    std::istream &fp = std::cin;
    //std::fstream fp("/Users/lore/Desktop/AlgoLab/AlgoLab/Week 8/week8/graypes/graypes_test.in");
    
    int n;
    
    while (fp >> n && n != 0) {
        
        std::vector<K::Point_2> delaunay_points;
        for (int i = 0; i < n; i++) {
            int x, y;
            fp >> x >> y;
            
            delaunay_points.push_back(K::Point_2(x, y));
        }
        
        Triangulation t(delaunay_points.begin(), delaunay_points.end());
    
        auto it = t.finite_edges_begin();
        auto end = t.finite_edges_end();
        
        
        auto shortest_segment = t.segment(it);
        for (it++; it != end; it++) {
            auto candidate_segment = t.segment(it);
            
            if (CGAL::compare_distance(candidate_segment.start(), candidate_segment.end(),
                                       shortest_segment.start(), shortest_segment.end()) == CGAL::SMALLER) {
                shortest_segment = candidate_segment;
            }
        }
        
        EPEC::Point_2 p1 = { shortest_segment.start().x(), shortest_segment.start().y() };
        EPEC::Point_2 p2 = { shortest_segment.end().x(), shortest_segment.end().y() };
        
        EPEC::Segment_2 epec_segment = EPEC::Segment_2(p1, p2);
        
        auto min_length = CGAL::sqrt(epec_segment.squared_length())*50;
        
        printf("%ld\n", ceil_to_int(min_length));
    }
    
    return 0;
}
