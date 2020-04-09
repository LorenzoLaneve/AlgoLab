//
//  Diet
//  AlgoLab
//
//  Copyright © 2019 Lorenzo Laneve. All rights reserved.
//

#include <iostream>
#include <fstream>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef int IT;
typedef CGAL::Gmpz ET;

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

#define units_of_product(j) (j)

#define nutrient_lower_bound_constraint(i) (2*(i))
#define nutrient_upper_bound_constraint(i) (2*(i)+1)

long floor_to_int(const CGAL::Quotient<ET>& x) {
    double a = std::floor(CGAL::to_double(x));
    while (a > x) a -= 1;
    while (a+1 <= x) a += 1;
    return (long) a;
}

int main(int argc, const char * argv[]) {
    
    std::istream &fp = std::cin;
    //std::fstream fp("/Users/lore/Desktop/AlgoLab/AlgoLab/Week 7/week7/diet/test.in");
    
    int n, m;
    while (fp >> n >> m && (n != 0 || m != 0)) {
        Program LP(CGAL::SMALLER, true, 0, false, 0);
        
        for (int i = 0; i < n; i++) {
            int min_i, max_i;
            fp >> min_i >> max_i;
            
            LP.set_b(nutrient_lower_bound_constraint(i), -min_i); // ∑ -C_ji x_j <= -min_i
            LP.set_b(nutrient_upper_bound_constraint(i), max_i); // ∑ C_ji x_j <= max_i
        }
        
        for (int j = 0; j < m; j++) {
            int p_j;
            fp >> p_j;
            
            LP.set_c(units_of_product(j), p_j);
            
            for (int i = 0; i < n; i++) {
                int C_ji;
                fp >> C_ji;
                
                LP.set_a(units_of_product(j), nutrient_lower_bound_constraint(i), -C_ji);
                LP.set_a(units_of_product(j), nutrient_upper_bound_constraint(i), C_ji);
            }
        }
        
        Solution s = CGAL::solve_linear_program(LP, ET());
        assert(s.solves_linear_program(LP));
        
        if (s.is_infeasible()) {
            printf("No such diet.\n");
        } else {
            printf("%ld\n", floor_to_int(s.objective_value()));
        }
        
    }
    
    return 0;
}
