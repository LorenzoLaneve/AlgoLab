//
//  World Cup
//  AlgoLab
//
//  Copyright © 2019 Lorenzo Laneve. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Delaunay_triangulation_2.h>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
#include <CGAL/Gmpq.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;

typedef long IT;
typedef CGAL::Gmpz ET;

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

#define r(w, s) (_r[m*((w)) + (s)])
#define t(w, s) (_t[m*((w)) + (s)])

#define f(w, s) (m*((w)) + (s))
#define supply_constraint(i) (i)
#define demand_upper_constraint(i) (n + (i))
#define demand_lower_constraint(i) (n + m + (i))
#define alcohol_constraint(i) (n + 2*m + (i))

typedef struct {
    long x;
    long y;

    long supply;
    long alcohol_perc;
} warehouse;

typedef struct {
    long x;
    long y;

    long demand;
    long alcohol_limit;
} stadium;

template <typename K> long floor_to_int(const K x) {
    double a = std::floor(CGAL::to_double(x));
    while (a > x) a -= 1;
    while (a+1 <= x) a += 1;
    return (long) a;
}
 
int main(int argc, const char * argv[]) {
    
    std::istream &fp = std::cin;
    //std::fstream fp("/Users/lore/Desktop/AlgoLab/AlgoLab/Week 13/week13/worldcup/test4.in");
    
    int testcases;
    fp >> testcases;
    
    for (int t = 0; t < testcases; t++) {
        long n, m, c;
        fp >> n >> m >> c;

        std::vector<K::Point_2> trp;

        std::vector<warehouse> whouses;
        for (long i = 0; i < n; i++) {
            long x, y, s, a;
            fp >> x >> y >> s >> a;

            whouses.push_back({ x, y, s, a });
            trp.push_back({ x, y });
        }

        std::vector<stadium> stadiums;
        for (long i = 0; i < m; i++) {
            long x, y, d, u;
            fp >> x >> y >> d >> u;

            stadiums.push_back({ x, y, d, u });
            trp.push_back({ x, y });
        }

        Triangulation tr(trp.begin(), trp.end());

        std::vector<long> _r(m*n);
        std::vector<long> _t(m*n, 0);
        for (long w = 0; w < n; w++) for (long s = 0; s < m; s++) {
            long r_ws; fp >> r_ws;
            r(w, s) = r_ws; // keep track of cents! 
        }

        for (long i = 0; i < c; i++) { // calculate t_ws
            long x, y, r;
            fp >> x >> y >> r;

            if (CGAL::compare_squared_distance(tr.nearest_vertex({ x, y })->point(), { x, y }, r*r) != CGAL::LARGER) {
                // if circle is empty we can ignore it!
                for (long w = 0; w < n; w++) for (long s = 0; s < m; s++) {
                    const auto &wh = whouses[w];
                    const auto &st = stadiums[s];

                    bool w_in = ((wh.x - x)*(wh.x - x) + (wh.y - y)*(wh.y - y) <= r*r);
                    bool s_in = ((st.x - x)*(st.x - x) + (st.y - y)*(st.y - y) <= r*r);

                    if (w_in != s_in) t(w, s)++; 
                }
            }
        }

        // Formulating LP: // 2000 variables, 260 constraints
        // max sum_ws (100*r_ws - t_ws) * f_ws
        // subject to sum_s f_ws <= supply_w for all w
        //            sum_w f_ws = demand_s for all s
        //            sum_w a_ws * f_ws <= 100*alcohol_limit_s for all s
        Program LP(CGAL::SMALLER, false, 0, false, 0);

        for (long w = 0; w < n; w++) for (long s = 0; s < m; s++) LP.set_c(f(w, s), t(w, s) - 100*r(w, s));

        for (long w = 0; w < n; w++) {
            for (long s = 0; s < m; s++) LP.set_a(f(w, s), supply_constraint(w), 1);
            LP.set_b(supply_constraint(w), whouses[w].supply);
        }

        for (long s = 0; s < m; s++) {
            for (long w = 0; w < n; w++) {
                LP.set_a(f(w, s), demand_upper_constraint(s), 1);
                LP.set_a(f(w, s), demand_lower_constraint(s), -1);
                LP.set_a(f(w, s), alcohol_constraint(s), whouses[w].alcohol_perc);
            }
            LP.set_b(demand_upper_constraint(s), stadiums[s].demand);
            LP.set_b(demand_lower_constraint(s), -stadiums[s].demand);
            LP.set_b(alcohol_constraint(s), 100 * stadiums[s].alcohol_limit);
        }

        Solution s = CGAL::solve_nonnegative_linear_program(LP, ET());
        if (s.is_infeasible()) std::cout << "RIOT!\n";
        else std::cout << floor_to_int(-s.objective_value()/100) << "\n";
    }
    
    return 0;
}
