//
//  Shortest Paths
//  AlgoLab
//
//  Copyright © 2019 Lorenzo Laneve. All rights reserved.
//

#include <iostream>
#include <limits>
#include <queue>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>


typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
boost::no_property, boost::property<boost::edge_weight_t, long> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;

#define infinity (std::numeric_limits<long>::max())

long dijkstra_dist(const weighted_graph &G, long n, long s, long t) {
    std::vector<long> dist_map(n, infinity);
    dist_map[s] = 0;

    auto w_map = boost::get(boost::edge_weight, G);

    auto order = [&](const std::pair<long, long> &i, const std::pair<long, long> &j) { return i.second > j.second; };

    std::priority_queue<std::pair<long, long>, std::vector<std::pair<long, long>>, decltype(order)> queue(order);
    queue.push({ s, 0 });
    
    while (!queue.empty()) {
        auto p = queue.top();
        queue.pop();

        long c = p.first;

        weighted_graph::out_edge_iterator it, end;
        for (boost::tie(it, end) = boost::out_edges(c, G); it != end; it++) {
            long neigh = boost::target(*it, G);

            long candidate_dist = dist_map[c] + w_map[*it];
            if (dist_map[neigh] > candidate_dist) {
                dist_map[neigh] = candidate_dist;
                queue.push({ neigh, candidate_dist });
            }
        }
    }

    return dist_map[t];
}

int main(int argc, const char * argv[]) {
    
    FILE *fp = stdin;//fopen("/Users/lore/Desktop/AlgoLab/AlgoLab/Week 4/week4/shortest_paths/bern.in", "r");
    
    long n, m, q;
    fscanf(fp, "%ld %ld %ld", &n, &m, &q);
    
    weighted_graph G(n);
    weight_map weights = boost::get(boost::edge_weight, G);
    
    for (int i = 0; i < n; i++) {
        long lat, lon;
        fscanf(fp, "%ld %ld", &lon, &lat);
    }
    
    for (int i = 0; i < m; i++) {
        long a, b, w;
        fscanf(fp, "%ld %ld %ld", &a, &b, &w);
        edge_desc e = boost::add_edge(a, b, G).first;
        
        weights[e] = w;
    }
    
    for (int i = 0; i < q; i++) {
        long s, t;
        fscanf(fp, "%ld %ld", &s, &t);
        
        long distance = dijkstra_dist(G, n, s, t);
        if (distance == infinity) {
            printf("unreachable\n");
        } else {
            printf("%ld\n", distance);
        }
    }
    
    return 0;
}
