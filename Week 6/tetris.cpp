//
//  Tetris
//  AlgoLab
//
//  Copyright © 2019 Lorenzo Laneve. All rights reserved.
//

#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>


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

#define start_vertex(i) (2*(i))
#define end_vertex(i) (2*(i)+1)

struct brick {
    int start;
    int end;
};

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
boost::property<boost::edge_capacity_t, long,
boost::property<boost::edge_residual_capacity_t, long,
boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;
typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;


int main(int argc, const char * argv[]) {
    
    FILE *fp = stdin;// fopen("/Users/lore/Desktop/AlgoLab/AlgoLab/Week 6/week6/tetris/bordercase.in", "r");
    
    int testcases;
    fscanf(fp, "%d", &testcases);
    
    for (int t = 0; t < testcases; t++) {
        int w, n;
        fscanf(fp, "%d %d", &w, &n);
     
        graph G(2*(w+1));
        const int source = start_vertex(0);
        const int target = end_vertex(w);
        
        auto c_map = boost::get(boost::edge_capacity, G);
        auto r_map = boost::get(boost::edge_reverse, G);
        
        for (int i = 1; i < w; i++) {
            add_unit_capacity_edge(end_vertex(i), start_vertex(i)); // vertex capacity 1
        }

        for (int i = 0; i < n; i++) {
            int start, end;
            fscanf(fp, "%d %d", &start, &end);
            
            if (start > end) std::swap(start, end);
            
            if (start != end) {
                add_unit_capacity_edge(start_vertex(start), end_vertex(end));
            }
        }
        
        long max_height = boost::push_relabel_max_flow(G, source, target);
        
        printf("%ld\n", max_height);
        
    }
    
    return 0;
}
