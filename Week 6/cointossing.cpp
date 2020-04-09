//
//  Coin Tossing
//  AlgoLab
//
//  Copyright © 2019 Lorenzo Laneve. All rights reserved.
//

#include <iostream>
#include <algorithm>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>


#define player_vertex(i) (i)
#define round_vertex(i) ((i)+n)

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
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;
typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

typedef enum {
    DONT_KNOW = 0,
    A_WINS = 1,
    B_WINS = 2
} round_outcome;

int main(int argc, const char * argv[]) {
    
    FILE *fp = stdin;//fopen("/Users/lore/Desktop/AlgoLab/AlgoLab/Week 6/week6/cointossing/test1a.in", "r");
    
    int testcases;
    fscanf(fp, "%d", &testcases);
    
    for (int t = 0; t < testcases; t++) {
        long n, m;
        fscanf(fp, "%ld %ld", &n, &m);
        
        graph G(n+m+2);
        auto c_map = boost::get(boost::edge_capacity, G);
        auto r_map = boost::get(boost::edge_reverse, G);
        
        const long source = n+m;
        const long target = n+m+1;
        
        for (int i = 0; i < m; i++) {
            int a, b, c;
            fscanf(fp, "%d %d %d", &a, &b, &c);
            
            add_unit_capacity_edge(source, round_vertex(i));
            
            switch (c) {
                case A_WINS:
                    add_unit_capacity_edge(round_vertex(i), player_vertex(a));
                    break;
                case B_WINS:
                    add_unit_capacity_edge(round_vertex(i), player_vertex(b));
                    break;
                default:
                    add_unit_capacity_edge(round_vertex(i), player_vertex(a));
                    add_unit_capacity_edge(round_vertex(i), player_vertex(b));
                    break;
            }
        }
        
        long sum = 0;
        for (int i = 0; i < n; i++) {
            int s_i;
            fscanf(fp, "%d", &s_i);
            
            sum += s_i;
            add_capacity_edge(player_vertex(i), target, s_i);
        }
        
        if (sum > m || boost::push_relabel_max_flow(G, source, target) != m) {
            printf("no\n");
        } else {
            printf("yes\n");
        }
    }
    
    return 0;
}
