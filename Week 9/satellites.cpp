//
//  Satellites
//  AlgoLab
//
//  Copyright © 2019 Lorenzo Laneve. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <queue>

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

#define add_capacity_edge(u, v, C) {\
    auto e = boost::add_edge((u), (v), G);\
    auto rev_e = boost::add_edge((v), (u), G);\
    \
    c_map[e.first] = (C);\
    c_map[rev_e.first] = 0;\
    r_map[rev_e.first] = e.first;\
    r_map[e.first] = rev_e.first;\
}


typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
boost::property<boost::edge_capacity_t, long,
boost::property<boost::edge_residual_capacity_t, long,
boost::property<boost::edge_reverse_t, traits::edge_descriptor,
boost::property<boost::edge_weight_t, long>>>>> graph;
typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

#define g(i) (i)
#define s(i) (g + (i))

int main(int argc, const char * argv[]) {
    
    std::istream &fp = std::cin;
    //std::fstream fp("/Users/lore/Desktop/AlgoLab/AlgoLab/Week 9/week9/satellites/test1.in");
    
    int testcases;
    fp >> testcases;
    
    for (int t = 0; t < testcases; t++) {
        long g, s, l;
        fp >> g >> s >> l;
        
        graph G(g + s + 2);
        const long source = g + s;
        const long target = g + s + 1;
        
        auto c_map = boost::get(boost::edge_capacity, G);
        auto r_map = boost::get(boost::edge_reverse, G);
        
        for (int i = 0; i < l; i++) {
            long a, b;
            fp >> a >> b;
            
            add_capacity_edge(g(a), s(b), 1);
        }
        
        for (int i = 0; i < g; i++) {
            add_capacity_edge(source, g(i), 1);
        }
        
        for (int i = 0; i < s; i++) {
            add_capacity_edge(s(i), target, 1);
        }
        
        boost::push_relabel_max_flow(G, source, target);
        
        auto rc_map = boost::get(boost::edge_residual_capacity, G);
        
        bool marked[g + s + 2];
        memset(marked, 0, sizeof(bool)*(g+s));
        
        std::queue<long> Q;
        marked[source] = true;
        
        Q.push(source);
        while (!Q.empty()) {
            const long u = Q.front();
            Q.pop();
            
            graph::out_edge_iterator it, end;
            for (boost::tie(it, end) = boost::out_edges(u, G); it != end; it++) {
                const long v = boost::target(*it, G);
                
                // Only follow edges with spare capacity
                if (!marked[v] && rc_map[*it] > 0) {
                    marked[v] = true;
                    Q.push(v);
                }
            }
        }
        
        long g_prime = 0;
        long s_prime = 0;
        for (int i = 0; i < g; i++) if (!marked[g(i)]) g_prime++;
        for (int i = 0; i < s; i++) if (marked[s(i)]) s_prime++;
        
        
        std::cout << g_prime << " " << s_prime << "\n";
        
        for (int i = 0; i < g; i++) if (!marked[g(i)]) std::cout << g(i) << " ";
        for (int i = 0; i < s; i++) if (marked[s(i)]) std::cout << i << " ";
        
        std::cout << "\n";
    }
    
    return 0;
}
