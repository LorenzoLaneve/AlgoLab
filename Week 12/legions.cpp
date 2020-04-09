//
//  Asterix and the Roman Lines
//  AlgoLab
//
//  Copyright © 2019 Lorenzo Laneve. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <cmath>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpq.h>
#include <CGAL/Gmpz.h>

typedef long IT;
typedef CGAL::Gmpq ET;

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

#define xs    0
#define ys    1
#define time  2

long floor_to_int(const CGAL::Quotient<ET>& x) {
    double a = std::floor(CGAL::to_double(x));
    while (a > x) a -= 1;
    while (a+1 <= x) a += 1;
    return (long) a;
}

int main(int argc, const char * argv[]) {
    
    std::istream &fp = std::cin;
    //std::fstream fp("/Users/lore/Desktop/AlgoLab/AlgoLab/Week 12/week12/legions/test1.in");
    
    int testcases;
    fp >> testcases;
    
    for (int t = 0; t < testcases; t++) {
        long x_s, y_s, n;
        fp >> x_s >> y_s >> n;

        Program LP(CGAL::SMALLER, false, 0, false, 0);
        LP.set_c(time, -1); // min -time <===> max time
        LP.set_l(time, true, 0);

        for (long i = 0; i < n; i++) {
            long a, b, c, v;
            fp >> a >> b >> c >> v;

            long D = (long) std::sqrt(a*a + b*b);
            assert(D*D == a*a + b*b);

            if (a*x_s < - b*y_s - c) {
                // ax + by + c < 0 for initial location ==> d(p, l) is negative
                // -d(p, l) >= v*time  <===> d(p, l) <= -v*time
                // ax + by + c <= -vD*time  <===> ax + by + vD*time <= -c

                LP.set_a(xs, i, a);
                LP.set_a(ys, i, b);
                LP.set_b(i, -c);
            } else {
                // ax + by + c > 0 for initial location ==> d(p, l) is positive
                // d(p, l) >= v*time
                // ax + by + c >= vD*time  <===> ax + by - vD*time >= -c  <===> -ax - by + vD*time <= c

                LP.set_a(xs, i, -a);
                LP.set_a(ys, i, -b);
                LP.set_b(i, c);
            }
            LP.set_a(time, i, v*D);
        }

        Solution sol = CGAL::solve_linear_program(LP, ET());
        assert(!sol.is_infeasible());

        std::cout << floor_to_int(-sol.objective_value()) << "\n";
    }
    
    return 0;
}
