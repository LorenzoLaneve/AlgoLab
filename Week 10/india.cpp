//
//  India
//  AlgoLab
//
//  Copyright © 2019 Lorenzo Laneve. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <limits>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/find_flow_cost.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>

#define add_unit_capacity_edge(u, v) {\
        auto e = boost::add_edge((u), (v), G);\
        auto rev_e = boost::add_edge((v), (u), G);\
        \
        c_map[e.first] = 1;\
        c_map[rev_e.first] = 0;\
        r_map[rev_e.first] = e.first;\
        r_map[e.first] = rev_e.first;\
    }

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

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
        boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
        boost::property<boost::edge_reverse_t, traits::edge_descriptor,
        boost::property<boost::edge_weight_t, long>>>>> graph;
typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

long right_bin_search(long L, long R, const std::function<bool(long)> &predicate) {

    while (L < R) {
        long mid = (L + R)/2;
        
        if (predicate(mid)) {
            L = mid + 1;
        } else {
            R = mid;
        }
    }

    return L - 1;
}

int main(int argc, const char * argv[]) {
    
    std::istream &fp = std::cin;
    //std::fstream fp("/Users/lore/Desktop/AlgoLab/AlgoLab/Week 10/week10/india/sample.in");
    
    int testcases;
    fp >> testcases;
    
    for (int t = 0; t < testcases; t++) {
        long c, g, b, k, a;
        fp >> c >> g >> b >> k >> a;

        graph G(c+2);
        auto c_map = boost::get(boost::edge_capacity, G);
        auto r_map = boost::get(boost::edge_reverse, G);
        auto w_map = boost::get(boost::edge_weight, G);

        for (long i = 0; i < g; i++) {
            long x, y, d, el;
            fp >> x >> y >> d >> el;

            add_capacity_edge(x, y, el, d);
        }

        const long source = c;
        const long target = c+1;

        add_capacity_edge(source, k, std::numeric_limits<long>::max(), 0);
        add_capacity_edge(a, target, std::numeric_limits<long>::max(), 0);
        auto src_edge = boost::edge(source, k, G).first;
        auto trg_edge = boost::edge(source, k, G).first;


        boost::successive_shortest_path_nonnegative_weights(G, source, target);
        const auto rc_map = boost::get(boost::edge_residual_capacity, G);

        long UB = c_map[trg_edge] - rc_map[trg_edge] + 1;
        // max luggages that can arrive using the network and infinite budget

        auto predicate = [&](long mid) {
            c_map[src_edge] = mid; // luggages that I want to transport

            boost::successive_shortest_path_nonnegative_weights(G, source, target);

            return boost::find_flow_cost(G) <= b;
        };

        std::cout << right_bin_search(0, UB, predicate) << "\n";
    }
    
    return 0;
}
