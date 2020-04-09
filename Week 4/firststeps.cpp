//
//  First Steps with BGL
//  AlgoLab
//
//  Copyright © 2019 Lorenzo Laneve. All rights reserved.
//

#include <iostream>
#include <queue>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
boost::no_property, boost::property<boost::edge_weight_t, int> >        weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;

typedef boost::graph_traits<weighted_graph>::out_edge_iterator out_edge_it;




long max_distance(weighted_graph &G, long n, long v) {
    std::vector<long> dist_map(n);
    
    boost::dijkstra_shortest_paths(G, v, boost::distance_map(boost::make_iterator_property_map(dist_map.begin(), boost::get(boost::vertex_index, G))));
    
    long distance = dist_map[0];
    
    for (int i = 1; i < n; i++) {
        if (distance < dist_map[i]) {
            distance = dist_map[i];
        }
    }
    
    return distance;
}


int main(int argc, const char *argv[]) {
    
    FILE *fp = stdin;//fopen("/Users/lore/Desktop/AlgoLab/AlgoLab/Week 4/week4/first_steps/sample.in", "r");
    
    int testcases;
    fscanf(fp, "%d", &testcases);
    
    for (int t = 0; t < testcases; t++) {
        int n, m;
        fscanf(fp, "%d %d", &n, &m);
        
        weighted_graph G(n);
        weight_map weights = boost::get(boost::edge_weight, G);
        
        for (int i = 0; i < m; i++) {
            int a, b, w;
            fscanf(fp, "%d %d %d", &a, &b, &w);
            
            edge_desc e, f;
            e = boost::add_edge(a, b, G).first;
            
            weights[e] = w;
        }
        
        std::vector<edge_desc> MST;
        
        boost::kruskal_minimum_spanning_tree(G, std::back_inserter(MST));
        
        
        weighted_graph GM(n);
        weight_map weightsM = boost::get(boost::edge_weight, GM);
        for (edge_desc e : MST) {
            edge_desc eM;
            eM = boost::add_edge(boost::source(e, G), boost::target(e, G), GM).first;
            
            weightsM[eM] = weights[e];
        }
        
        long MST_weight = 0;
        for (edge_desc e : MST) {
            MST_weight += weights[e];
        }
        
        
        bool visited[n];
        for (int i = 0; i < n; i++) {
            visited[i] = false;
        }

        long max_d = max_distance(G, n, 0);
        
        printf("%ld %ld\n", MST_weight, max_d);
    }
    
    return 0;
}
