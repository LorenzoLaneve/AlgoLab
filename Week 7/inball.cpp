//
//  Inball
//  AlgoLab
//
//  Copyright © 2019 Lorenzo Laneve. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <cmath>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef int IT;
typedef CGAL::Gmpz ET;

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;


#define x(j) (j)
#define wall_constraint(i) (n+(i))
#define radius_constraint(i) (i)

#define R (d)

int floor_to_int(const CGAL::Quotient<ET>& x) {
    double a = std::floor(CGAL::to_double(x));
    while (a > x) a -= 1;
    while (a+1 <= x) a += 1;
    return (int) a;
}

int main(int argc, const char * argv[]) {
    
    std::istream &fp = std::cin;
    //std::fstream fp("/Users/lore/Desktop/AlgoLab/AlgoLab/Week 7/week7/inball/test.in");
    
    int n, d;
    while (fp >> n >> d && n != 0) {
        Program LP(CGAL::SMALLER, false, 0, false, 0);
        
        for (int i = 0; i < n; i++) {
            
            CGAL::Gmpz squared_norm = 0;
            
            for (int j = 0; j < d; j++) {
                int a_ij;
                fp >> a_ij;
                
                LP.set_a(x(j), wall_constraint(i), a_ij);
                LP.set_a(x(j), radius_constraint(i), a_ij);
                
                squared_norm += a_ij * a_ij;
            }
            
            int b_i;
            fp >> b_i;
            
            LP.set_b(wall_constraint(i), b_i);
            LP.set_b(radius_constraint(i), b_i);
            
            LP.set_a(R, radius_constraint(i), floor_to_int(CGAL::sqrt(squared_norm)));
        }
        
        LP.set_c(R, -1); // min -r
        
        Solution s = CGAL::solve_linear_program(LP, ET());
        if (s.is_infeasible()) {
            printf("none\n");
        } else if (s.is_unbounded()) {
            printf("inf\n");
        } else {
            printf("%d\n", floor_to_int(-s.objective_value()));
        }
    }
    
    return 0;
}
