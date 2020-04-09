//
//  Radiation
//  AlgoLab
//
//  Copyright © 2019 Lorenzo Laneve. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <cmath>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef CGAL::Gmpz IT;
typedef CGAL::Gmpz ET;

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

struct cell {
    long x;
    long y;
    long z;
};


bool distinguishable(int d, const std::vector<cell> &healthy_cells, const std::vector<cell> &tumor_cells) {
    Program LP(CGAL::SMALLER, false, 0, false, 0);
    
    int constr_id = 0;
    for (const cell &c : healthy_cells) {
        // p(x) <= -1
        
        int var_id = 0;
        IT x_p = 1;
        for (long dx = 0; dx <= d; dx++) {
            IT y_p = 1;
            for (long dy = 0; dy <= d - dx; dy++) {
                IT z_p = 1;
                for (long dz = 0; dz <= d - dx - dy; dz++) {
                    LP.set_a(var_id++, constr_id, x_p * y_p * z_p);
                    z_p *= c.z;
                }
                y_p *= c.y;
            }
            x_p *= c.x;
        }
        
        LP.set_b(constr_id++, -1);
    }
    
    for (const cell &c : tumor_cells) {
        // p(x) >= 1
        
        int var_id = 0;
        IT x_p = 1;
        for (long dx = 0; dx <= d; dx++) {
            IT y_p = 1;
            for (long dy = 0; dy <= d - dx; dy++) {
                IT z_p = 1;
                for (long dz = 0; dz <= d - dx - dy; dz++) {
                    LP.set_a(var_id++, constr_id, -1 * x_p * y_p * z_p);
                    z_p *= c.z;
                }
                y_p *= c.y;
            }
            x_p *= c.x;
        }
        
        LP.set_b(constr_id++, -1);
    }
    
    CGAL::Quadratic_program_options options;
    options.set_pricing_strategy(CGAL::QP_BLAND);
    Solution s = CGAL::solve_linear_program(LP, ET(), options);
    
    return s.is_optimal();
}


int main(int argc, const char * argv[]) {
    
    std::istream &fp = std::cin;
    //std::fstream fp("/Users/lore/Desktop/AlgoLab/AlgoLab/Week 7/week7/radiation/test.in");
    
    int testcases;
    fp >> testcases;
    
    for (int tn = 0; tn < testcases; tn++) {
        int h, t;
        fp >> h >> t;
        
        std::vector<cell> healthy_cells;
        std::vector<cell> tumor_cells;
    
        for (int i = 0; i < h; i++) {
            int x, y, z;
            fp >> x >> y >> z;
            
            healthy_cells.push_back({ x, y, z });
        }
        
        for (int i = 0; i < t; i++) {
            int x, y, z;
            fp >> x >> y >> z;
            
            tumor_cells.push_back({ x, y, z });
        }
        

        
#ifdef BIN_SEARCH
        int i = 0;
        int j = 30;

        while (i <= j && i <= 30) {
            int mid = (i + j) / 2;

            if (distinguishable(mid, healthy_cells, tumor_cells)) {
                j = mid - 1;
            } else {
                i = mid + 1;
            }
        }

        i = std::max(i, 0);
#else
        int i;
        for (i = 0; i <= 30; i++) {
            if (distinguishable(i, healthy_cells, tumor_cells)) {
                break;
            }
        }
#endif
        
        if (i > 30) {
            std::cout << "Impossible!\n";
        } else {
            std::cout << i << "\n";
        }
        
    }
    
    return 0;
}
