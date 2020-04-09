//
//  Surveillance Photographs
//  AlgoLab
//
//  Copyright © 2019 Lorenzo Laneve. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <limits>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

#define infinity (std::numeric_limits<long>::max())

#define add_unit_capacity_edge(u, v) {\
    auto e = boost::add_edge((u), (v), G);\
    auto rev_e = boost::add_edge((v), (u), G);\
    \
    c_map[e.first] = 1;\
    c_map[rev_e.first] = 0;\
    r_map[rev_e.first] = e.first;\
    r_map[e.first] = rev_e.first;\
}

#define add_capacity_edge(u, v, C) {\
    auto e = boost::add_edge((u), (v), G);\
    auto rev_e = boost::add_edge((v), (u), G);\
    \
    c_map[e.first] = (C);\
    c_map[rev_e.first] = 0;\
    r_map[rev_e.first] = e.first;\
    r_map[e.first] = rev_e.first;\
}

#define before_vertex(i) (i)
#define after_vertex(i) (n + i)


typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
boost::property<boost::edge_capacity_t, long,
boost::property<boost::edge_residual_capacity_t, long,
boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;
typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

int main(int argc, const char * argv[]) {
    
    std::istream &fp = std::cin;
    //std::fstream fp("/Users/lore/Desktop/AlgoLab/AlgoLab/Week 11/week11/surveillance/test2.in");
    
    int testcases;
    fp >> testcases;
    
    for (int t = 0; t < testcases; t++) {
        long n, m, k, l;
        fp >> n >> m >> k >> l;

        graph G(2*n + 2); // create two maps. one before and one after picking the photograph
        auto c_map = boost::get(boost::edge_capacity, G);
        auto r_map = boost::get(boost::edge_reverse, G);

        const long source = 2*n;
        const long target = 2*n + 1;

        for (int i = 0; i < k; i++) { // police stations
            long x; fp >> x;
            
            add_unit_capacity_edge(source, before_vertex(x)); // one policeman
            add_unit_capacity_edge(after_vertex(x), target);  // one safe
        }

        for (int i = 0; i < l; i++) { // photographs
            long x; fp >> x;
            
            add_unit_capacity_edge(before_vertex(x), after_vertex(x)); // pass from "before" to "after", only one photograph
        }

        for (int i = 0; i < m; i++) {
            long x, y;
            fp >> x >> y;

            add_capacity_edge(before_vertex(x), before_vertex(y), infinity);
            add_unit_capacity_edge(after_vertex(x), after_vertex(y));
        }

        long max_pickable = boost::push_relabel_max_flow(G, source, target);
        std::cout << max_pickable << "\n";
    }
    
    return 0;
}
