//
//  Bob's Burden
//  AlgoLab
//
//  Copyright © 2019 Lorenzo Laneve. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <limits>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property, boost::property<boost::edge_weight_t, long>> graph;
typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

#define b(i, j) ((i)*((i)+1)/2 + (j))
#define b_in(i, j) (2*b(i, j))
#define b_out(i, j) (2*b(i, j) + 1)

#define add_edge_w(u, v, W) {\
    auto e = boost::add_edge((u), (v), G);\
    w_map[e.first] = (W);\
}


int main(int argc, const char * argv[]) {
    
    std::istream &fp = std::cin;
    //std::fstream fp("/Users/lore/Desktop/AlgoLab/AlgoLab/Week 13/week13/bobsburden/test2.in");
    
    int testcases;
    fp >> testcases;
    
    for (int t = 0; t < testcases; t++) {
        long k; fp >> k;

        const long num_presents = k*(k+1)/2;
        graph G(num_presents * 2); // vertex costs
        auto w_map = boost::get(boost::edge_weight, G);

        // for (long i = 0; i < k; i++) for (long j = 0; j <= i; j++) std::cout << b_in(i, j) << " " << b_out(i, j) << " ";
        // std::cout << source << " " << target << "\n";

        for (long i = 0; i < k; i++) for (long j = 0; j <= i; j++) {
            long v_ij; fp >> v_ij;

            // add edge from in to out with cost v_ij
            add_edge_w(b_in(i, j), b_out(i, j), v_ij);
        }

        // add zero-cost edges between any pair of neighboring balls
        for (long i = 0; i < k; i++) for (long j = 0; j <= i; j++) {
            // (i-1, j), (i-1, j-1) OK
            if (i > 0) {
                if (j < i) add_edge_w(b_out(i, j), b_in(i-1, j), 0);
                if (j > 0) add_edge_w(b_out(i, j), b_in(i-1, j-1), 0);
            }

            // (i, j-1), (i, j+1)   OK
            if (j < i) add_edge_w(b_out(i, j), b_in(i, j+1), 0);
            if (j > 0) add_edge_w(b_out(i, j), b_in(i, j-1), 0);
            
            // (i+1, j), (i+1, j+1) OK
            if (i < k - 1) {
                add_edge_w(b_out(i, j), b_in(i+1, j), 0);
                add_edge_w(b_out(i, j), b_in(i+1, j+1), 0);
            }
        }

        std::vector<long> d1(num_presents * 2);
        std::vector<long> d2(num_presents * 2);
        std::vector<long> d3(num_presents * 2);

        boost::dijkstra_shortest_paths(G, b_in(0, 0), boost::distance_map(boost::make_iterator_property_map(d1.begin(), boost::get(boost::vertex_index, G))));
        boost::dijkstra_shortest_paths(G, b_in(k-1, 0), boost::distance_map(boost::make_iterator_property_map(d2.begin(), boost::get(boost::vertex_index, G))));
        boost::dijkstra_shortest_paths(G, b_in(k-1, k-1), boost::distance_map(boost::make_iterator_property_map(d3.begin(), boost::get(boost::vertex_index, G))));


        long min_weight = std::numeric_limits<long>::max(); // look for the center of mass (i.e. the node minimizing d1+d2+d3)
        for (long i = 0; i < k; i++) for (long j = 0; j <= i; j++) {
            min_weight = std::min(min_weight, d1[b_out(i, j)] + d2[b_in(i, j)] + d3[b_in(i, j)]);
            // take into account the center's weight only once! (i.e. only one out)
        }
        
        std::cout << min_weight << "\n";
    }
    
    return 0;
}
