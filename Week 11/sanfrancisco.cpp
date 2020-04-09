//
//  San Francisco
//  AlgoLab
//
//  Copyright © 2019 Lorenzo Laneve. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <algorithm>

#include <boost/graph/adjacency_list.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
boost::no_property, boost::property<boost::edge_weight_t, long> >       graph;
typedef boost::graph_traits<graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<graph>::vertex_descriptor          vertex_desc;

struct edge {
    long u;
    long v;
    long p;
};

#define tab(i, j) (_tab[n*(j) + (i)])

long play(graph &G, long k, long x) {
    auto scores = boost::get(boost::edge_weight, G);
    long n = boost::num_vertices(G);

    std::vector<long> _tab(n*(k+1), -1);
    for (long i = 0; i < n; i++) tab(i, 0) = 0;

    for (long j = 1; j <= k; j++) for (long v = 0; v < n; v++) {
        graph::out_edge_iterator it, end;
        
        long max_score = -1;
        for (boost::tie(it, end) = boost::out_edges(v, G); it != end; it++) {
            long neigh = boost::target(*it, G);

            assert(tab(neigh, j - 1) >= 0);
            max_score = std::max(max_score, tab(neigh, j - 1) + scores[*it]);
        }
        tab(v, j) = max_score;
    }

    for (long i = 0; i <= k; i++) if (tab(0, i) >= x) return i;
    
    return k+1;
}

int main(int argc, const char * argv[]) {
    
    std::istream &fp = std::cin;
    //std::fstream fp("/Users/lore/Desktop/AlgoLab/AlgoLab/Week 11/week11/sanfrancisco/test1.in");
    
    int testcases;
    fp >> testcases;
    
    for (int t = 0; t < testcases; t++) {
        long n, m, x, k;
        fp >> n >> m >> x >> k;

        graph G(n);
        auto scores = boost::get(boost::edge_weight, G);

        const long start = 0;
        std::vector<bool> is_sink(n, true);
        std::vector<edge> edges(m);
        for (long i = 0; i < m; i++) {
            fp >> edges[i].u >> edges[i].v >> edges[i].p;
            is_sink[edges[i].u] = false;
        }

        for (edge &es : edges) {
            if (is_sink[es.v]) es.v = start;

#ifdef UNIQUE_EDGES
            auto e = boost::edge(es.u, es.v, G);
            if (!e.second) {
                auto new_e = boost::add_edge(es.u, es.v, G);
                scores[new_e.first] = es.p;
            } else {
                scores[e.first] = std::max(scores[e.first], es.p);
            }
#else
            auto new_e = boost::add_edge(es.u, es.v, G);
            scores[new_e.first] = es.p;
#endif
        }

        long min_moves = play(G, k, x);
        if (min_moves > k) {
            std::cout << "Impossible\n";
        } else {
            std::cout << min_moves << "\n";
        }
    }
    
    return 0;
}
