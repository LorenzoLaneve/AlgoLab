//
//  Canteen
//  AlgoLab
//
//  Copyright © 2019 Lorenzo Laneve. All rights reserved.
//

#include <iostream>
#include <fstream>

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

#define offer_vertex(i) ((i))

int main(int argc, const char * argv[]) {
    
    std::istream &fp = std::cin;
    //std::fstream fp("/Users/lore/Desktop/AlgoLab/AlgoLab/Week 9/week9/canteen/test3.in");
    
    int testcases;
    fp >> testcases;
    
    for (int t = 0; t < testcases; t++) {
        int n;
        fp >> n;
        
        graph G(n + 2);
        auto c_map = boost::get(boost::edge_capacity, G);
        auto r_map = boost::get(boost::edge_reverse, G);
        auto w_map = boost::get(boost::edge_weight, G);
        
        const int source = n;
        const int target = n + 1;
        
        for (int i = 0; i < n; i++) {
            int a_i, c_i;
            fp >> a_i >> c_i;
            
            add_capacity_edge(source, offer_vertex(i), a_i, c_i);
        }
        
        long total_students = 0;
        for (int i = 0; i < n; i++) {
            int s_i, p_i;
            fp >> s_i >> p_i;
            
            add_capacity_edge(offer_vertex(i), target, s_i, -p_i + 20);
            
            total_students += s_i;
        }
        
        for (int i = 0; i < n - 1; i++) {
            int v_i, e_i;
            fp >> v_i >> e_i;
            
            add_capacity_edge(offer_vertex(i), offer_vertex(i+1), v_i, e_i);
        }

        boost::successive_shortest_path_nonnegative_weights(G, source, target);
        const auto rc_map = boost::get(boost::edge_residual_capacity, G);
        
        long flow = 0;
        graph::out_edge_iterator it, end;
        for (boost::tie(it, end) = boost::out_edges(source, G); it != end; it++) {
            flow += c_map[*it] - rc_map[*it];
        }
        
        long cost = boost::find_flow_cost(G) - 20*flow;
        
        
        if (flow == total_students) {
            std::cout << "possible";
        } else {
            std::cout << "impossible";
        }
        
        
        std::cout << " " << flow << " " << -cost << "\n";
    }
    
    return 0;
}
