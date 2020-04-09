//
//  Real Estate Market
//  AlgoLab
//
//  Copyright © 2019 Lorenzo Laneve. All rights reserved.
//

#include <iostream>
#include <fstream>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > graph;

typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it; // Iterator

class edge_adder {
 graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}
  void add_edge(int from, int to, long capacity, long cost) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto w_map = boost::get(boost::edge_weight, G); // new!
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
    w_map[e] = cost;   // new assign cost
    w_map[rev_e] = -cost;   // new negative cost
  }
};

#define state_vertex(i) (i)
#define site_vertex(i) (s + (i))
#define buyer_vertex(i) (s + m + (i))

int main(int argc, const char * argv[]) {
    
    std::istream &fp = std::cin;
    //std::fstream fp("/Users/lore/Desktop/AlgoLab/AlgoLab/Week 9/week9/real_estate/test1.in");
    
    int testcases;
    fp >> testcases;
    
    for (int t = 0; t < testcases; t++) {
        long n, m, s;
        fp >> n >> m >> s;

        graph G(s + m + n + 2);
        edge_adder adder(G);

        const long source = s + m + n;
        const long target = s + m + n + 1;

        for (long i = 0; i < s; i++) {
            long l_i; fp >> l_i;
            adder.add_edge(source, state_vertex(i), l_i, 0);
        }

        for (long j = 0; j < m; j++) {
            long s_j; fp >> s_j;
            adder.add_edge(state_vertex(s_j - 1), site_vertex(j), 1, 0);
        }

        for (long i = 0; i < n; i++) { 
            adder.add_edge(buyer_vertex(i), target, 1, 0);
            for (long j = 0; j < m; j++) {
                long b_ij; fp >> b_ij;
                adder.add_edge(site_vertex(j), buyer_vertex(i), 1, 100 - b_ij);
            }
        }

        boost::successive_shortest_path_nonnegative_weights(G, source, target);

        auto c_map = boost::get(boost::edge_capacity, G);
        auto rc_map = boost::get(boost::edge_residual_capacity, G);

        graph::out_edge_iterator it, end;
        long flow = 0;
        for (boost::tie(it, end) = boost::out_edges(source, G); it != end; it++) flow += c_map[*it] - rc_map[*it];

        std::cout << flow << " " << 100 * flow - boost::find_flow_cost(G) << "\n";
    }
    
    return 0;
}
