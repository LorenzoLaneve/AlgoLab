//
//  Germs
//  AlgoLab
//
//  Copyright © 2019 Lorenzo Laneve. All rights reserved.
//

#include <iostream>
#include <fstream>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/nearest_neighbor_delaunay_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Triangulation::Finite_faces_iterator Face_iterator;

typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt KS;

long ceil_to_int(KS::FT x) {
    double a = std::floor(CGAL::to_double(x));
    while (a < x) a += 1;
    while (a-1 >= x) a -= 1;
    return (long) a;
}

int main(int argc, const char * argv[]) {
    
    std::istream &fp = std::cin;
    //std::fstream fp("/Users/lore/Desktop/AlgoLab/AlgoLab/Week 8/week8/germs/germs_test.in");
    
    int n;
    while (fp >> n && n != 0) {
        int l, b, r, t;
        fp >> l >> b >> r >> t;
        
        std::vector<K::Point_2> bacteria;
        for (int i = 0; i < n; i++) {
            long x, y;
            fp >> x >> y;
            
            bacteria.push_back({ x, y });
        }
        
        Triangulation tr(bacteria.begin(), bacteria.end());
        
        auto it = tr.finite_vertices_begin();
        auto end = tr.finite_vertices_end();
        
        
        std::vector<K::FT> decay_distances;
        for (; it != end; it++) {
            auto dish_limit_x = CGAL::min(CGAL::abs(l - it->point().x()), CGAL::abs(r - it->point().x()));
            auto dish_limit_y = CGAL::min(CGAL::abs(t - it->point().y()), CGAL::abs(b - it->point().y()));
            
            auto d = CGAL::min(dish_limit_x, dish_limit_y) * 2;
            d *= d;
            
            if (n > 1) {
                auto closest_germ = CGAL::nearest_neighbor(tr, it);
                auto closest_germ_dist = CGAL::squared_distance(closest_germ->point(), it->point());
                
                if (d > closest_germ_dist) {
                    d = closest_germ_dist;
                }
            }

            decay_distances.push_back(d);
        }
        
        std::sort(decay_distances.begin(), decay_distances.end());
        
        KS::FT first_d = KS::FT(decay_distances[0]);
        KS::FT half_d = KS::FT(decay_distances[n/2]);
        KS::FT last_d = KS::FT(decay_distances[n-1]);
        
        // rho(t) = 1/2 + t^2 = d/2 => t^2 = (d - 1)/2
        long first_decay = ceil_to_int(CGAL::sqrt((CGAL::sqrt(first_d) - 1)/2));
        long half_decay =  ceil_to_int(CGAL::sqrt((CGAL::sqrt(half_d) - 1)/2));
        long total_decay = ceil_to_int(CGAL::sqrt((CGAL::sqrt(last_d) - 1)/2));
        
        std::cout << first_decay << " " << half_decay << " " << total_decay << "\n";
    }
    
    return 0;
}
