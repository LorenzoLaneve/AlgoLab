//
//  Shopping
//  AlgoLab
//
//  Copyright © 2019 Lorenzo Laneve. All rights reserved.
//

#include <iostream>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>


#define add_unit_capacity_edge(u, v) {\
    auto e = boost::add_edge((u), (v), G);\
    auto rev_e = boost::add_edge((v), (u), G);\
    \
    c_map[e.first] = 1;\
    c_map[rev_e.first] = 1;\
    r_map[rev_e.first] = e.first;\
    r_map[e.first] = rev_e.first;\
}

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
                                boost::property<boost::edge_capacity_t, long,
                                boost::property<boost::edge_residual_capacity_t, long,
                                boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;
typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

int main(int argc, const char * argv[]) {
    
    FILE *fp = stdin;//fopen("/Users/lore/Desktop/AlgoLab/AlgoLab/Week 6/week6/shopping/sample2.in", "r");
    
    int testcases;
    fscanf(fp, "%d", &testcases);
    
    for (int t = 0; t < testcases; t++) {
        long n, m, s;
        fscanf(fp, "%ld %ld %ld", &n, &m, &s);
        
        graph G(n+1);
        long target_vertex = n;
        
        auto c_map = boost::get(boost::edge_capacity, G);
        auto r_map = boost::get(boost::edge_reverse, G);
        
        for (long i = 0; i < s; i++) {
            long v;
            fscanf(fp, "%ld", &v);
            
            add_unit_capacity_edge(v, target_vertex);
        }
        
        for (long i = 0; i < m; i++) {
            long from, to;
            fscanf(fp, "%ld %ld", &from, &to);
            
            add_unit_capacity_edge(from, to);
        }
        
        long max_flow = boost::push_relabel_max_flow(G, 0, target_vertex);
        
        if (max_flow >= s) {
            printf("yes\n");
        } else {
            printf("no\n");
        }
        
        
    }
    
    return 0;
}
