//
//  Planet Express
//  AlgoLab
//
//  Copyright © 2019 Lorenzo Laneve. All rights reserved.
//

#include <iostream>
#include <cstring>
#include <limits>
#include <algorithm>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/strong_components.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>


typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::bidirectionalS,
boost::no_property, boost::property<boost::edge_weight_t, long> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;

// all distances from t
std::vector<long> dijkstra_distances(const weighted_graph &G, long n, long t) {
    std::vector<long> dist_map(2*n);
    
    boost::dijkstra_shortest_paths(G, t, boost::distance_map(boost::make_iterator_property_map(dist_map.begin(), boost::get(boost::vertex_index, G))));
    
    return dist_map;
}

int main(int argc, const char * argv[]) {
    
    FILE *fp = stdin;//fopen("/Users/lore/Desktop/AlgoLab/AlgoLab/Week 5/week5/planetexpress/tests/test2.in", "r");
    
    int testcases;
    fscanf(fp, "%d", &testcases);
    
    for (int t = 0; t < testcases; t++) {
        int n, m, k, T;
        fscanf(fp, "%d %d %d %d", &n, &m, &k, &T);
        
        bool *is_in_teleport_network = new bool[n];
        memset(is_in_teleport_network, 0, sizeof(bool)*n);
        
        weighted_graph *G = new weighted_graph(2*n);
        weight_map travel_times = boost::get(boost::edge_weight, *G);
        
        for (int i = 0; i < T; i++) {
            int t_i;
            fscanf(fp, "%d", &t_i);
            
            is_in_teleport_network[t_i] = true;
        }
        
        for (int i = 0; i < m; i++) {
            int u, v;
            long c;
            fscanf(fp, "%d %d %ld", &u, &v, &c);
            edge_desc e = boost::add_edge(v, u, *G).first;
            // REVERSED EDGES
            
            travel_times[e] = c;
        }

        
        std::vector<int> scc_map(2*n); // scc_map[i] is the index of the strongly connected component containing vertex i
        // nscc: total number of SCCs
        int nscc = boost::strong_components(*G, boost::make_iterator_property_map(scc_map.begin(), boost::get(boost::vertex_index, *G)));
        
        
        std::vector<std::vector<int>> scc(nscc);
        for (int i = 0; i < n; i++) if (is_in_teleport_network[i]) {
            scc[scc_map[i]].push_back(i);
        }
        
        int scc_vertex = n;
        for (auto &component : scc) if (component.size() > 1) {
            for (int v : component) {
                assert(is_in_teleport_network[v]);
                edge_desc e = boost::add_edge(scc_vertex, v, *G).first;
                // REVERSED EDGES

                travel_times[e] = component.size() - 1;
                
                
                edge_desc ret = boost::add_edge(v, scc_vertex, *G).first;
                // REVERSED EDGES
                
                travel_times[ret] = 0;
            }
            scc_vertex++;
        }
        
        // calculate dijkstra from first k vertices to vertex n - 1
        long min_time = std::numeric_limits<long>::max();
        
        
        
        const int persei8 = n - 1;
        std::vector<long> distances = dijkstra_distances(*G, n, persei8);
        for (int i = 0; i < k; i++) {
            long time_from_i = distances[i];
            
            if (min_time > time_from_i) min_time = time_from_i;
        }
        
        if (min_time > 1000000) {
            printf("no\n");
        } else {
            printf("%ld\n", min_time);
        }
        
        //delete G;
        delete[] is_in_teleport_network;
    }
    
    return 0;
}
