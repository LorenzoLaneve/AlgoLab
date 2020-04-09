
#include <iostream>
#include <fstream>
#include <vector>

int main(int argc, const char *argv[]) {
    std::cin.sync_with_stdio(false);
    std::fstream fp("/Users/lore/Desktop/AlgoLab/AlgoLab/Threefolds/tests/moretorcycles3.in");

    int testcases;
    fp >> testcases;
    for (int tc = 0; tc < n; tc++) {
        long n, m, w; fp >> n >> m >> w;
        
        std::vector<biker> bikers;
        for (long i = 0; i < n; i++) {
            long y0, x1, y1; fp >> y0 >> x1 >> y1;
            bikers.push_back({ K::Point_2(0, y0), K::Ray_2(K::Point_2(0, y0), K::Point_2(x1, y1)) });
        }

        std::vector<K::Point_2> antennas;
        for (long i = 0; i < m; i++) {
            long x, y; fp >> x >> y;
            antennas.push_back({ x, y });
        }

        


    }

    return 0;
}