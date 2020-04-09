
#include <iostream>
#include <fstream>
#include <vector>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;

struct biker {
    long start;
    K::FT slope;
};

int main(int argc, const char *argv[]) {
    std::cin.sync_with_stdio(false);
    //std::istream &fp = std::cin;
    std::fstream fp("/Users/lore/Desktop/AlgoLab/AlgoLab/Threefolds/tests/moretorcycles1.in");

    int testcases;
    fp >> testcases;
    for (int tc = 0; tc < testcases; tc++) {
        long n; fp >> n;

        std::vector<biker> bikers;
        for (long i = 0; i < n; i++) {
            long y0, x1, y1;
            fp >> y0 >> x1 >> y1;

            K::Direction_2 dir = K::Line_2(K::Point_2(0, y0), K::Point_2(x1, y1)).direction();

            bikers.push_back({ y0, dir.dy()/dir.dx() });
        }

        std::sort(bikers.begin(), bikers.end(), [&](const biker &i, const biker &j) { return i.start > j.start; });

        std::vector<long> stack;
        stack.push_back(0);
        for (long i = 1; i < n; i++) {
            bool should_add_i = true;
            while (!stack.empty() && bikers[stack.back()].slope < bikers[i].slope) {
                // the two bikers intersect somewhere
                if (CGAL::abs(bikers[stack.back()].slope) >= CGAL::abs(bikers[i].slope)) {
                    // i comes first and kills back
                    stack.pop_back();
                } else {
                    // back comes first and kills i
                    should_add_i = false;
                    break;
                }
            }

            if (should_add_i) stack.push_back(i);
        }

        std::cout << stack.size() << " ";

        long lis_len = 1;
        std::vector<K::FT> F(n+1);
        F[1] = -bikers[0].slope;

        for (long i = 1; i < n; i++) {
            K::FT elem_i = -bikers[i].slope;

            // look for L such that F[L - 1] <= b[i] <= F[L]
            long L = 1, R = lis_len + 1;
            while (L < R) {
                long mid = (L + R)/2;

                if (elem_i > F[mid]) {
                    L = mid + 1;
                } else R = mid;
            }

            F[L] = elem_i;
            lis_len = std::max(lis_len, L);
        }

        std::cout << lis_len << "\n";
    }

    return 0;
}