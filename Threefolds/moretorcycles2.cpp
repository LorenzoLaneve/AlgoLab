
#include <iostream>
#include <fstream>
#include <vector>

#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpq.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;

typedef K::FT IT;
typedef K::FT ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;


template <typename K> long floor_to_double(const K& x) {
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return (long) a;
}

template <typename K> long ceil_to_double(const K& x) {
    return -floor_to_double(-x);
}

#define f (n)
#define slack(i) (i)

struct biker {
    K::Point_2 start;
    K::Ray_2 ray;
};

int main(int argc, const char *argv[]) {
    std::cin.sync_with_stdio(false);
    std::fstream fp("/Users/lore/Desktop/AlgoLab/AlgoLab/Threefolds/tests/moretorcycles2.in");

    int testcases;
    fp >> testcases;
    for (int tc = 0; tc < testcases; tc++) {
        long n; fp >> n;

        Program LP(CGAL::SMALLER, true, 0, false, 0);

        std::vector<biker> bikers;
        for (long i = 0; i < n; i++) {
            long y0, x1, y1; fp >> y0 >> x1 >> y1;
            bikers.push_back({ K::Point_2(0, y0), K::Ray_2(K::Point_2(0, y0), K::Point_2(x1, y1)) });
        }

        long constr_id = 0;
        for (long i = 0; i < n; i++) for (long j = i+1; j < n; j++) {
            // d1 - d2 + s1 - s2 <= f
            // d2 - d1 + s2 - s1 <= f
            // => s1 - s2 - f <= d2 - d1
            // => s2 - s1 - f <= d1 - d2
            if (CGAL::do_intersect(bikers[i].ray, bikers[j].ray)) {
                auto is = CGAL::intersection(bikers[i].ray, bikers[j].ray);

                if (const K::Point_2* p = boost::get<K::Point_2>(&*is)) { // always passes
                    K::FT d1 = CGAL::sqrt(CGAL::squared_distance(bikers[i].start, *p));
                    K::FT d2 = CGAL::sqrt(CGAL::squared_distance(bikers[j].start, *p));

                    LP.set_a(slack(i), constr_id, 1);
                    LP.set_a(slack(j), constr_id, -1);
                    LP.set_a(f, constr_id, -1);
                    LP.set_b(constr_id++, d2 - d1);

                    LP.set_a(slack(j), constr_id, 1);
                    LP.set_a(slack(i), constr_id, -1);
                    LP.set_a(f, constr_id, -1);
                    LP.set_b(constr_id++, d1 - d2);
                }
            }
        }

        LP.set_c(f, 1);

        Solution s = CGAL::solve_linear_program(LP, ET());
        std::cout << ceil_to_double(s.objective_value()) << "\n";
    }
    return 0;
}