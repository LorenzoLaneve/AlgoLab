//
//  The Empire Strikes Back
//  AlgoLab
//
//  Copyright © 2019 Lorenzo Laneve. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
#include <algorithm>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpq.h>
#include <CGAL/Gmpz.h>

#define infinity (std::numeric_limits<long>::max())

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Triangulation::Finite_faces_iterator Face_iterator;

typedef CGAL::Gmpq IT;
typedef CGAL::Gmpq ET;

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

struct particle {
    long x;
    long y;
    long d;
};

#define e(i) (i)
#define particle_constraint(i) (i)
#define energy_constraint (a)

int main(int argc, const char * argv[]) {
    
    std::istream &fp = std::cin;
    //std::fstream fp("/Users/lore/Desktop/AlgoLab/AlgoLab/Week 11/week11/strikesback/sample_test1.in");
    
    int testcases;
    fp >> testcases;
    
    for (int t = 0; t < testcases; t++) {
        long a, s, b, e;
        fp >> a >> s >> b >> e;

        std::vector<particle> particles(a);
        std::vector<K::Point_2> spoints(s);
        std::vector<K::Point_2> hunters(b);

        for (long i = 0; i < a; i++) fp >> particles[i].x >> particles[i].y >> particles[i].d;
        for (long i = 0; i < s; i++) {
            long x, y; fp >> x >> y;
            spoints[i] = { x, y };
        } 
        for (long i = 0; i < b; i++) {
            long x, y; fp >> x >> y;
            hunters[i] = { x, y };
        } 

        Triangulation htr(hunters.begin(), hunters.end());
        std::vector<K::FT> max_radius(s, infinity); // strict upper bound for the i-th shot

        Program LP(CGAL::LARGER, true, 0, false, 0);
        for (long j = 0; j < a; j++) LP.set_b(particle_constraint(j), particles[j].d);


        for (long i = 0; i < s; i++) {
            const K::Point_2 &sp = spoints[i];
            if (b > 0) {
                max_radius[i] = CGAL::squared_distance(htr.nearest_vertex(sp)->point(), sp);
                // radius of i-th shot must be strictly smaller than the distance to the nearest hunter
            }
            
            for (long j = 0; j < a; j++) {
                const particle &pt = particles[j];
                K::FT squared_distance = CGAL::squared_distance(sp, { pt.x, pt.y });
                if (squared_distance < max_radius[i]) {
                    // this particle can be struck by this shot

                    // add 1/max(1, squared_distance) to the coefficient of e_i for the j-th constraint of the LP
                    CGAL::Gmpq a_ij = CGAL::Gmpq(1)/std::max(1.0, squared_distance);
                    LP.set_a(e(i), particle_constraint(j), a_ij);
                }
            }
        }

        // set up last constraint: sum of e_i <= e  <===> sum of -e_i >= -e
        for (long i = 0; i < s; i++) LP.set_a(e(i), energy_constraint, -1);
        LP.set_b(energy_constraint, -e);

        Solution sol = CGAL::solve_linear_program(LP, ET());
        std::cout << (sol.is_infeasible() ? "n\n" : "y\n");
    }
    
    return 0;
}
