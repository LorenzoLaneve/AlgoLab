//
//  Bistro
//  AlgoLab
//
//  Copyright © 2019 Lorenzo Laneve. All rights reserved.
//

#include <iostream>
#include <fstream>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Triangulation::Finite_faces_iterator Face_iterator;


long ceil_to_int(K::FT x) {
    double a = CGAL::to_double(x);
    while (a < x) a += 1;
    while (a-1 >= x) a -= 1;
    return (long) a;
}

int main(int argc, const char * argv[]) {
    
    std::istream &fp = std::cin;
    //std::fstream fp("/Users/lore/Desktop/AlgoLab/AlgoLab/Week 8/week8/bistro/bistro_test.in");
    
    int n, m;
    while (fp >> n && n != 0) {
        
        std::vector<K::Point_2> delaunay_points;
        for (int i = 0; i < n; i++) {
            int x, y;
            fp >> x >> y;
            
            delaunay_points.push_back(K::Point_2(x, y));
        }
        
        Triangulation t(delaunay_points.begin(), delaunay_points.end());
        
        fp >> m;
        for (int i = 0; i < m; i++) {
            int x, y;
            fp >> x >> y;
            
            auto nearest_point = t.nearest_vertex(K::Point_2(x, y))->point();
            
            auto shortest_segment = K::Segment_2({ x, y }, nearest_point);
            
            printf("%ld\n", ceil_to_int(shortest_segment.squared_length()));
        }
    }
    
    return 0;
}
