//
//  New Tiles
//  AlgoLab
//
//  Copyright © 2019 Lorenzo Laneve. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <vector>

#define tile_vertex(i, j) (h*(j) + (i))

#define memo(lc, r) ((*_memo)[(h*(lc) + (r))])

std::vector<long> *_memo;
std::vector<long> *_vc;

// r is the row we are considering
// last_config is the configuration of the previous row (1 iff cell is free)
long max_tiles(long last_config, long r, long w, long h, const std::vector<long> &rows) {
    if (r >= h - 1) return 0; // floor has weight 1 => no tiles can be placed

    if (memo(last_config, r) >= 0) return memo(last_config, r);

    // cells where you can actually put a tile (its corner)
    // row & (row >> 1) gives a row where one can see in the i-th bit if i and i+1 cells are free
    long available_cells = rows[r] & (rows[r] >> 1) & rows[r+1] & (rows[r+1] >> 1);
    available_cells &= last_config & (last_config >> 1);

    const long N = 1 << w;

    long max = 0;
    for (long c : *_vc) { // 1 iff a tile corner is placed there
        if ((c & available_cells) == c) { // c places tiles only from available tiles
            long occupied_tiles = c | (c << 1); // as a tile also occupies the next cell
            occupied_tiles = (~occupied_tiles) & ((1 << w) - 1); // invert bits: occupied cells must be 0

            max = std::max(max, __builtin_popcount(c) + max_tiles(occupied_tiles, r+1, w, h, rows));
        }
    }

    return memo(last_config, r) = max;
}

int main(int argc, const char * argv[]) {
    //std::istream &fp = std::cin;
    std::fstream fp("/Users/lore/Desktop/AlgoLab/AlgoLab/Week 10/week10/newtiles/test2.in");
    
    int testcases;
    fp >> testcases;

    std::vector<long> vc[18];
    for (int t = 0; t < testcases; t++) {
        long h, w; fp >> h >> w;

        std::vector<long> rows(h);
        for (long i = 0; i < h; i++) {
            for (long j = 0; j < w; j++) {
                long x; fp >> x;
                rows[i] |= (x) << j; // 1 iff a tile can be placed here (without taking account for the next row)
            }
        }

        std::vector<long> m((1 << w) * h, -1);
        
        _memo = &m;
        _vc = &vc[w];
        
        const long N = 1 << (w - 2);
        if (vc[w].empty()) for (long c = 0; c < N; c++) if (!(c & (c >> 1))) vc[w].push_back(c);

        // (1 << w) - 1 // w times 1 === all cells are free
        long result = max_tiles((1 << w) - 1, 0, w, h, rows);
        std::cout << result << "\n";

        _memo = nullptr;
        _vc = nullptr;
    }
    
    return 0;
}
