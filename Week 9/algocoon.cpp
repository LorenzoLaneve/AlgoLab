//
//  Algocoon
//  AlgoLab
//
//  Copyright © 2019 Lorenzo Laneve. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <limits>
#include <queue>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
//#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
//#include <boost/graph/find_flow_cost.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
boost::property<boost::edge_capacity_t, long,
boost::property<boost::edge_residual_capacity_t, long,
boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;
typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;


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

#define infinity std::numeric_limits<long>::max()

int main(int argc, const char * argv[]) {
    
    //std::istream &fp = std::cin;
    std::fstream fp("/Users/lore/Desktop/AlgoLab/AlgoLab/Week 9/week9/algocoon/test4.in");

    int testcases;
    fp >> testcases;
    
    for (int t = 0; t < testcases; t++) {
        int n, m;
        fp >> n >> m;
        
        graph G(n);
        
        auto c_map = boost::get(boost::edge_capacity, G);
        auto r_map = boost::get(boost::edge_reverse, G);
        
        for (int i = 0; i < m; i++) {
            long a, b, c;
            fp >> a >> b >> c;
            
            add_capacity_edge(a, b, c);
        }
        
        long min_cut = infinity;
        int min_u = 0;
        int min_v = 1;
        
        for (int v = 1; v < n; v++) {
            long candidate_cut = boost::push_relabel_max_flow(G, 0, v);
            
            if (candidate_cut < min_cut) {
                min_cut = candidate_cut;
                min_u = 0;
                min_v = v;
            }
        }
        
        for (int u = 1; u < n; u++) {
            long candidate_cut = boost::push_relabel_max_flow(G, u, 0);
            
            if (candidate_cut < min_cut) {
                min_cut = candidate_cut;
                min_u = u;
                min_v = 0;
            }
        }
        
        
        long final_cut = boost::push_relabel_max_flow(G, min_u, min_v);
        assert(final_cut == min_cut);
        
        auto rc_map = boost::get(boost::edge_residual_capacity, G);
        
        std::queue<long> Q;
        bool visited[n];
        memset(visited, 0, sizeof(bool)*n);
        visited[min_u] = true;
        long figures = 1;
        
        Q.push(min_u);
        while (!Q.empty()) {
            const long v = Q.front();
            Q.pop();
            
            graph::out_edge_iterator it, end;
            for (boost::tie(it, end) = boost::out_edges(v, G); it != end; it++) {
                const auto neigh = boost::target(*it, G);
                
                if (rc_map[*it] > 0 && !visited[neigh]) {
                    visited[neigh] = true;
                    figures++;
                    Q.push(neigh);
                }
            }
        }
        
        std::cout << final_cut << "\n" << figures;
        for (int i = 0; i < n; i++) if (visited[i]) std::cout << " " << i;
        std::cout << "\n";
    }
    
    return 0;
}
