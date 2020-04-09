//
//  Marathon
//  AlgoLab
//
//  Copyright © 2019 Lorenzo Laneve. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <queue>
#include <limits>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/find_flow_cost.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>

#define infinity std::numeric_limits<long>::max()

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
    \
    c_map[e.first] = (C);\
    w_map[e.first] = (W);\
}


typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
    boost::property<boost::edge_residual_capacity_t, long,
    boost::property<boost::edge_reverse_t, traits::edge_descriptor,
    boost::property<boost::edge_weight_t, long>>>>> graph;
typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

int main(int argc, const char * argv[]) {
    
    std::istream &fp = std::cin;
    //std::fstream fp("/Users/lore/Desktop/AlgoLab/AlgoLab/Week 9/week9/marathon/test4.in");
    
    int testcases;
    fp >> testcases;
    
    for (int t = 0; t < testcases; t++) {
        int n, m, s, f;
        fp >> n >> m >> s >> f;
        
        graph G(n);
        const int source = s;
        const int target = f;
        
        auto c_map = boost::get(boost::edge_capacity, G);
        auto r_map = boost::get(boost::edge_reverse, G);
        auto w_map = boost::get(boost::edge_weight, G);
        
        long capacity_sum = 0;
        long length_upper_bound = 0;
        for (int i = 0; i < m; i++) {
            int a, b, c, d;
            fp >> a >> b >> c >> d;
            
            add_capacity_edge(a, b, c, d);
            add_capacity_edge(b, a, c, d);
            
            capacity_sum += c;
            length_upper_bound += d;
        }
        
        length_upper_bound += 1; // upper bound on path lengths
        
        std::vector<long> distances(n);
        boost::dijkstra_shortest_paths(G, s, boost::distance_map(boost::make_iterator_property_map(distances.begin(), boost::get(boost::vertex_index, G))));
        
        std::vector<edge_desc> edges;
        
        graph::edge_iterator it, end;
        for (boost::tie(it, end) = boost::edges(G); it != end; it++) if (w_map[*it] != length_upper_bound) {
            edges.push_back(*it);
        }
        
        
        for (edge_desc e : edges) {
            if (distances[boost::source(e, G)] + w_map[e] > distances[boost::target(e, G)]) {
                boost::remove_edge(e, G);
            }
        }
        
        edges.clear();
        for (boost::tie(it, end) = boost::edges(G); it != end; it++) {
            edges.push_back(*it);
        }
        
        for (edge_desc e : edges) {
            auto v = boost::target(e, G);
            auto u = boost::source(e, G);
            
            auto rev_e = boost::add_edge(v, u, G);
            
            c_map[rev_e.first] = 0;
            r_map[rev_e.first] = e;
            r_map[e] = rev_e.first;
            w_map[rev_e.first] = -w_map[e];
        }
        
        long flow = boost::push_relabel_max_flow(G, source, target);
        
        /*
        boost::successive_shortest_path_nonnegative_weights(G, source, target);
        
        const auto rc_map = boost::get(boost::edge_residual_capacity, G);
        
        
        graph::out_edge_iterator it, end;
        long flow = 0;
        for (boost::tie(it, end) = boost::out_edges(source, G); it != end; it++) {
            flow += c_map[*it] - rc_map[*it];
        }*/
        
        std::cout << flow << "\n";
    }
    
    return 0;
}
