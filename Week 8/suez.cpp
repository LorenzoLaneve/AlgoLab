//
//  Suez
//  AlgoLab
//
//  Copyright © 2019 Lorenzo Laneve. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <limits>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpq.h>
#include <CGAL/Gmpz.h>

typedef long IT;
typedef CGAL::Gmpq ET;

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

#define a(i) (i)

#define infinity (std::numeric_limits<IT>::max())

struct nail {
    int x;
    int y;
};

long ceil_to_int(const CGAL::Quotient<ET>& x) {
    double a = std::ceil(CGAL::to_double(x));
    while (a < x) a += 1;
    while (a-1 >= x) a -= 1;
    return (long) a;
}

int main(int argc, const char * argv[]) {
    
    std::istream &fp = std::cin;
    //std::fstream fp("/Users/lore/Desktop/AlgoLab/AlgoLab/Week 8/week8/suez/test3.in");
    
    int testcases;
    fp >> testcases;
    
    for (int t = 0; t < testcases; t++) {
        long n, m, h, w;
        fp >> n >> m >> h >> w;
        
        Program LP(CGAL::SMALLER, false, 0, false, 0);
        
        std::vector<nail> new_nails;
        for (int i = 0; i < n; i++) {
            int x, y;
            fp >> x >> y;
            
            new_nails.push_back({ x, y });
        }
        
        std::vector<nail> old_nails;
        for (int i = 0; i < m; i++) {
            int x, y;
            fp >> x >> y;
            
            old_nails.push_back({ x, y });
        }
        
        int constraint_id = 0;
        for (int i = 0; i < new_nails.size(); i++) {
            
            const nail &nail_i = new_nails[i];
            for (int j = i+1; j < new_nails.size(); j++) {
                const nail &nail_j = new_nails[j];
                
                if (h*abs(nail_i.x - nail_j.x) >= w*abs(nail_i.y - nail_j.y)) { // add constraint to x
                    // x_i + a_i w/2 <= x_j - a_j w/2  ===>  w a_i + w a_j <= 2 |x_j - x_i|
                    // x_j + a_j w/2 <= x_i - a_i w/2
                    LP.set_a(a(i), constraint_id, w);
                    LP.set_a(a(j), constraint_id, w);
                    
                    LP.set_b(constraint_id++, 2*abs(nail_i.x - nail_j.x));
                }
                
                if (h*abs(nail_i.x - nail_j.x) <= w*abs(nail_i.y - nail_j.y)) { // add constraint to y
                    // y_i + a_i h/2 <= y_j - a_j h/2  ===>  h a_i + h a_j <= 2 |y_j - y_i|
                    // y_j + a_j h/2 <= y_i - a_i h/2
                    LP.set_a(a(i), constraint_id, h);
                    LP.set_a(a(j), constraint_id, h);
                    
                    LP.set_b(constraint_id++, 2*abs(nail_i.y - nail_j.y));
                }
            }
            
            
            long w_tightest_bound = infinity;
            long h_tightest_bound = infinity;
            
            for (int j = 0; j < old_nails.size(); j++) {
                const nail &nail_j = old_nails[j];
                
                if (h*abs(nail_i.x - nail_j.x) >= w*abs(nail_i.y - nail_j.y)) { // add constraint to x
                    // x_i + a_i w/2 <= x_j - w/2  ===> a_i w <= 2*|x_j - x_i| - w
                    // x_i - a_i w/2 >= x_j + w/2
                    
                    long candidate_bound = 2*abs(nail_i.x - nail_j.x) - w;
                    if (w_tightest_bound > candidate_bound) {
                        w_tightest_bound = candidate_bound;
                    }
                } else { // add constraint to y
                    // y_i + a_i h/2 <= y_j - h/2  ===> a_i h <= 2*|y_j - y_i| - h
                    // y_i - a_i h/2 >= y_j + h/2
                    
                    long candidate_bound = 2*abs(nail_i.y - nail_j.y) - h;
                    if (h_tightest_bound > candidate_bound) {
                        h_tightest_bound = candidate_bound;
                    }
                }
            }
            
            if (w_tightest_bound < infinity) { // a_i w <= min( 2*|x_j - x_i| - w )
                LP.set_a(a(i), constraint_id, w);
                LP.set_b(constraint_id++, w_tightest_bound);
            }
            
            if (h_tightest_bound < infinity) { // a_i h <= min( 2*|y_j - y_i| - h )
                LP.set_a(a(i), constraint_id, h);
                LP.set_b(constraint_id++, h_tightest_bound);
            }
            
        }
        
        for (int i = 0; i < n; i++) {
            LP.set_c(a(i), -(2*h + 2*w));
            LP.set_l(a(i), true, 1);
        }
        
        Solution s = CGAL::solve_linear_program(LP, ET());
        assert(!s.is_unbounded());
        assert(!s.is_infeasible());
        
        std::cout << ceil_to_int(-s.objective_value()) << "\n";
    }
    
    return 0;
}
