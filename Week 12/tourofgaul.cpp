//
//  Asterix and the Tour of Gaul
//  AlgoLab
//
//  Copyright © 2019 Lorenzo Laneve. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>


typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
boost::property<boost::edge_capacity_t, long,
boost::property<boost::edge_residual_capacity_t, long,
boost::property<boost::edge_reverse_t, traits::edge_descriptor,
boost::property<boost::edge_weight_t, long>>>>> graph;
typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

#define add_capacity_edge(u, v, C, W) {\
    auto e = boost::add_edge((u), (v), G);\
    auto rev_e = boost::add_edge((v), (u), G);\
    \
    c_map[e.first] = (C);\
    c_map[rev_e.first] = 0;\
    r_map[rev_e.first] = e.first;\
    r_map[e.first] = rev_e.first;\
    w_map[e.first] = (W);\
    w_map[rev_e.first] = -(W);\
}

#define hl(a, b) (hard_limit[n*(a) + (b)])
#define fd(a, b) (food_table[n*(a) + (b)])

void compute_limits(long i, long j, long n, std::vector<long> &hard_limit, const std::vector<long> &c) {
    if (i >= j) return;

    // look for bottleneck between i and j
    long bottleneck = 101;
    long bk_loc = -1;
    for (long k = i; k < j; k++) {
        if (bottleneck > c[k]) {
            bottleneck = c[k];
            bk_loc = k;
        }
    }

    for (long l = i; l <= bk_loc; l++) for (long r = bk_loc+1; r <= j; r++) {
        hl(l, r) = bottleneck;
    }

    compute_limits(i, bk_loc, n, hard_limit, c);
    compute_limits(bk_loc+1, j, n, hard_limit, c);
}

int main(int argc, const char * argv[]) {
    
    std::istream &fp = std::cin;
    //std::fstream fp("/Users/lore/Desktop/AlgoLab/AlgoLab/Week 12/week12/tourofgaul/test5.in");
    
    int testcases;
    fp >> testcases;
    
    for (int t = 0; t < testcases; t++) {
        long n, m;
        fp >> n >> m;

        graph G(n+2);
        auto c_map = boost::get(boost::edge_capacity, G);
        auto r_map = boost::get(boost::edge_reverse, G);
        auto w_map = boost::get(boost::edge_weight, G);

        const long source = n;
        const long target = n+1;

        const long U = 128;
        std::vector<long> c(n - 1);
        for (long i = 0; i < n - 1; i++) {
            fp >> c[i];
            add_capacity_edge(i, i+1, c[i], U);

            add_capacity_edge(source, i, c[i], 0);
            add_capacity_edge(i+1, target, c[i], 0);
        }

        std::vector<long> hard_limit(n*n, 100); // max number of elements from a to b (bottleneck)
        compute_limits(0, n-1, n, hard_limit, c); 

        std::vector<std::priority_queue<long, std::vector<long>>> food_table(n*n);
        for (long i = 0; i < m; i++) {
            long a, b, d; fp >> a >> b >> d;

            fd(a, b).push(-d);
            if (fd(a, b).size() > hl(a, b)) fd(a, b).pop();
        }


        for (long a = 0; a < n; a++) for (long b = a+1; b < n; b++) {
            while (!fd(a, b).empty()) {
                long d = -fd(a, b).top();
                fd(a, b).pop();

                add_capacity_edge(a, b, 1, U*(b - a) - d);
            }
        }

        boost::successive_shortest_path_nonnegative_weights(G, source, target);
        long min_cost = boost::find_flow_cost(G);
        
        auto rc_map = boost::get(boost::edge_residual_capacity, G);
        long flow = 0;

        graph::out_edge_iterator it, end;
        for (boost::tie(it, end) = boost::out_edges(source, G); it != end; it++) {
            flow += c_map[*it] - rc_map[*it];
        }

        std::cout << U*flow - min_cost << "\n";
    }
    
    return 0;
}
