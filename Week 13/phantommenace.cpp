//
//  The Phantom Menace
//  AlgoLab
//
//  Copyright © 2019 Lorenzo Laneve. All rights reserved.
//

#include <iostream>
#include <fstream>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
    boost::property<boost::edge_residual_capacity_t, long,
    boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;
typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

#define add_capacity_edge(u, v, C) {\
    auto e = boost::add_edge((u), (v), G);\
    auto rev_e = boost::add_edge((v), (u), G);\
    \
    c_map[e.first] = (C);\
    c_map[rev_e.first] = 0;\
    r_map[rev_e.first] = e.first;\
    r_map[e.first] = rev_e.first;\
}

#define in_v(i) (2*(i))
#define out_v(i) (2*(i)+1)

int main(int argc, const char * argv[]) {
    
    std::istream &fp = std::cin;
    //std::fstream fp("/Users/lore/Desktop/AlgoLab/AlgoLab/Week 13/week13/phantom_menace/test3.in");
    
    int testcases;
    fp >> testcases;
    
    for (int t = 0; t < testcases; t++) {
        long n, m, s, d;
        fp >> n >> m >> s >> d;

        graph G(2 * n + 2);
        auto c_map = boost::get(boost::edge_capacity, G);
        auto r_map = boost::get(boost::edge_reverse, G);

        const long source = 2 * n;
        const long target = 2 * n + 1;

        for (long i = 0; i < n; i++) add_capacity_edge(in_v(i), out_v(i), 1);

        for (long i = 0; i < m; i++) {
            long u, v; fp >> u >> v;
            add_capacity_edge(out_v(u), in_v(v), 1);
        }

        for (long i = 0; i < s; i++) {
            long u; fp >> u;
            add_capacity_edge(source, in_v(u), 1);
        }

        for (long i = 0; i < d; i++) {
            long u; fp >> u;
            add_capacity_edge(out_v(u), target, 1);
        }

        long min_cut = boost::push_relabel_max_flow(G, source, target);

        std::cout << min_cut << "\n";
    }
    
    return 0;
}
