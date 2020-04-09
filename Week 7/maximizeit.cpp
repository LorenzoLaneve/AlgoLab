//
//  Maximize it!
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

#define X 0
#define Y 1
#define Z 2


long floor_to_int(const CGAL::Quotient<ET>& x) {
    double a = std::floor(CGAL::to_double(x));
    while (a > x) a -= 1;
    while (a+1 <= x) a += 1;
    return (long) a;
}

long ceil_to_int(const CGAL::Quotient<ET>& x) {
    double a = std::ceil(CGAL::to_double(x));
    while (a < x) a += 1;
    while (a-1 >= x) a -= 1;
    return (long) a;
}



int main(int argc, const char * argv[]) {
    
    std::istream &fp = std::cin;
    //std::ifstream fp("/Users/lore/Desktop/AlgoLab/AlgoLab/Week 7/week7/maximizeit/sample.in");
    
    int p, a, b;
    while (fp >> p >> a >> b && p != 0) {
        if (p == 1) {
            Program LP(CGAL::SMALLER, true, 0, false, 0); // x, y >= 0
            
            LP.set_a(X, 0, 1);
            LP.set_a(Y, 0, 1);
            LP.set_b(0, 4); // x + y <= 4
            
            LP.set_a(X, 1, 4);
            LP.set_a(Y, 1, 2);
            LP.set_b(1, a*b); // 4x + 2y <= ab
        
            LP.set_a(X, 2, -1);
            LP.set_a(Y, 2, 1);
            LP.set_b(2, 1); // -x + y <= 1
            
            LP.set_c(X, a);
            LP.set_c(Y, -b); // max by - ax == min - by + ax
            
            Solution s = CGAL::solve_linear_program(LP, ET());
            if (s.is_infeasible()) {
                printf("no\n");
            } else if (s.is_unbounded()) {
                printf("unbounded\n");
            } else {
                printf("%ld\n", floor_to_int(-s.objective_value()));
            }
        } else {
            Program LP(CGAL::LARGER, false, 0, true, 0); // x, y <= 0
            
            LP.set_a(X, 0, 1);
            LP.set_a(Y, 0, 1);
            LP.set_b(0, -4); // x + y >= -4
            
            LP.set_a(X, 1, 4);
            LP.set_a(Y, 1, 2);
            LP.set_a(Z, 1, 1);
            LP.set_b(1, -a*b); // 4x + 2y + z >= -ab
            
            LP.set_a(X, 2, -1);
            LP.set_a(Y, 2, 1);
            LP.set_b(2, -1); // -x + y >= -1
            
            LP.set_c(X, a);
            LP.set_c(Y, b);
            LP.set_c(Z, 1); // min ax + by + z
            
            Solution s = CGAL::solve_linear_program(LP, ET());
            if (s.is_infeasible()) {
                printf("no\n");
            } else if (s.is_unbounded()) {
                printf("unbounded\n");
            } else {
                printf("%ld\n", ceil_to_int(s.objective_value()));
            }
        }
        
        
        

    }

    return 0;
}
