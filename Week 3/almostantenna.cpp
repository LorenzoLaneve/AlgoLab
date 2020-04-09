//
//  Almost Antenna
//  AlgoLab
//
//  Copyright © 2019 Lorenzo Laneve. All rights reserved.
//

#include <iostream>
#include <algorithm>

#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>


typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt EPEC;


double ceil_to_int(const EPEC::FT& x) {
    double a = std::ceil(CGAL::to_double(x));
    while (a-1 > x) a -= 1;
    while (a < x) a += 1;
    return a;
}

int main(int argc, const char *argv[]) {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    
    int n;
    do {
        std::cin >> n;
        
        if (n) {
            std::vector<EPEC::Point_2> pointset(n);
            for (int i = 0; i < n; i++) {
                double x, y;
                std::cin >> x >> y;
                
                pointset[i] = EPEC::Point_2(x, y);
            }
            
            CGAL::Min_circle_2<CGAL::Min_circle_2_traits_2<EPEC>> min_c(pointset.begin(), pointset.end(), true);
            
            EPEC::FT min_radius = min_c.circle().squared_radius();
            for (int j = 0; j < min_c.number_of_support_points(); j++) {
                auto it = min_c.support_point(j);
                
                for (int i = 0; i < pointset.size(); i++) {
                    if (pointset[i] == it) {
                        pointset.erase(pointset.begin()+i, pointset.begin()+i+1);
                        i--;
                    }
                }
                
                if (pointset.size() > 0) {
                    CGAL::Min_circle_2<CGAL::Min_circle_2_traits_2<EPEC>> almost_min_c(pointset.begin(), pointset.end(), true);
                    EPEC::FT radius = almost_min_c.circle().squared_radius();
                    if (min_radius > radius) {
                        min_radius = radius;
                    }
                }
                
                pointset.push_back(it);
            }
            
            std::cout << std::fixed << std::setprecision(0) << ceil_to_int(CGAL::sqrt(min_radius)) << "\n";
        }
    } while (n);
    
    return 0;
}
