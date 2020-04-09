//
//  Ant Challenge
//  AlgoLab
//
//  Copyright © 2019 Lorenzo Laneve. All rights reserved.
//

#include <iostream>
#include <limits>
#include <map>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
boost::no_property, boost::property<boost::edge_weight_t, long> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;


long dijkstra_dist(const weighted_graph &G, long n, long s, long t) {
    std::vector<long> dist_map(n);
    
    boost::dijkstra_shortest_paths(G, s, boost::distance_map(boost::make_iterator_property_map(dist_map.begin(), boost::get(boost::vertex_index, G))));
    
    return dist_map[t];
}

inline void load_map(weight_map &graph_map, const std::map<edge_desc, long> &map) {
    for (auto &entry : map) {
        graph_map[entry.first] = entry.second;
    }
}

int main(int argc, const char * argv[]) {
    
    FILE *fp = stdin;//fopen("/Users/lore/Desktop/AlgoLab/AlgoLab/Week 4/week4/ant_challenge/test1.in", "r");
    
    int testcases;
    fscanf(fp, "%d", &testcases);
    
    for (int t = 0; t < testcases; t++) {
        int n, e, s, a, b;
        fscanf(fp, "%d %d %d %d %d", &n, &e, &s, &a, &b);
        
        weighted_graph forest_network(n);
        weight_map graph_map = boost::get(boost::edge_weight, forest_network);
        
        std::map<edge_desc, long> travel_times[s];
        
        std::map<edge_desc, long> min_travel_times;
        for (int i = 0; i < e; i++) {
            long t1, t2;
            fscanf(fp, "%ld %ld", &t1, &t2);
            
            edge_desc edge = boost::add_edge(t1, t2, forest_network).first;
            
            
            min_travel_times[edge] = std::numeric_limits<int>::max();
            for (int j = 0; j < s; j++) {
                long w_j;
                fscanf(fp, "%ld", &w_j);
                
                travel_times[j][edge] = w_j;
            }
        }
        
        for (int i = 0; i < s; i++) {
            int h_i;
            fscanf(fp, "%d", &h_i);
            
            load_map(graph_map, travel_times[i]);
            
            // calc MST
            std::vector<edge_desc> MST;
            boost::kruskal_minimum_spanning_tree(forest_network, std::back_inserter(MST));
            
            for (edge_desc e : MST) {
                if (min_travel_times[e] > travel_times[i][e]) {
                    min_travel_times[e] = travel_times[i][e];
                }
            }
        }
//        
//        weighted_graph::edge_iterator it, end;
//        for (boost::tie(it, end) = boost::edges(forest_network); it != end; it++) {
//            printf("%ld -> %ld: %ld\n", boost::source(*it, forest_network), boost::target(*it, forest_network), min_travel_times[*it]);
//        }

        load_map(graph_map, min_travel_times);
        printf("%ld\n", dijkstra_dist(forest_network, n, a, b));
    }
    
    return 0;
}
