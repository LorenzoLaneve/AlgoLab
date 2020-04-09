//
//  Cantonal Courier
//  AlgoLab
//
//  Copyright © 2019 Lorenzo Laneve. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <limits>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

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

#define job_vertex(i) (i)
#define zone_vertex(i) (J + (i))

#define infinity std::numeric_limits<long>::max()

int main(int argc, const char * argv[]) {
    
    std::istream &fp = std::cin;
    //std::fstream fp("/Users/lore/Desktop/AlgoLab/AlgoLab/Week 7/week7/courier/test3.in");
    
    int testcases;
    fp >> testcases;
    
    for (int t = 0; t < testcases; t++) {
        int Z, J;
        fp >> Z >> J;
        
        graph G(Z+J+2);
        const int source = Z+J;
        const int target = Z+J+1;
        
        auto c_map = boost::get(boost::edge_capacity, G);
        auto r_map = boost::get(boost::edge_reverse, G);
        
        
        long total_reward = 0;
    
        for (int i = 0; i < Z; i++) {
            int c_i;
            fp >> c_i;
            
            add_capacity_edge(zone_vertex(i), target, c_i);
        }
        
        for (int i = 0; i < J; i++) {
            int p_i;
            fp >> p_i;
            
            total_reward += p_i;
            add_capacity_edge(source, job_vertex(i), p_i);
        }
        
        for (int i = 0; i < J; i++) {
            int N_i;
            fp >> N_i;
            
            for (int j = 0; j < N_i; j++) {
                int zone_id;
                fp >> zone_id;
                
                add_capacity_edge(job_vertex(i), zone_vertex(zone_id), infinity);
            }
        }
        
        // min possible lose
        long min_cut_size = boost::push_relabel_max_flow(G, source, target);
        
        printf("%ld\n", total_reward - min_cut_size);
    }
    
    return 0;
}
