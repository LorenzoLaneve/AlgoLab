//
//  Return of the Jedi
//  AlgoLab
//
//  Copyright © 2019 Lorenzo Laneve. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <queue>

#include <boost/pending/disjoint_sets.hpp>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
//#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
//#include <boost/graph/find_flow_cost.hpp>



typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
        boost::property<boost::edge_weight_t, long>> graph;
typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

struct link_t {
    long from_v;
    long new_v;

    long cost;
};

#define c(i, j) (costs[n*(std::max((i), (j))-1) + std::min((i), (j))-1])
#define hv(i, j) (_heaviest[n*(std::max((i), (j))-1) + std::min((i), (j))-1])
#define am(i, j) (_adj_m[n*(std::max((i), (j))-1) + std::min((i), (j))-1])

// Computes the mst of K_n. We do not need to build it like in the assignment since,
// if there are more than one mst the algorithm will find the other one and conclude anyway that the cost is still the minimum.
long leia_mst(long *costs, long n, long i, std::vector<bool> &_adj_m, std::vector<long> &_heaviest) {
    boost::disjoint_sets_with_storage<> uf(n+1);
    std::vector<bool> visited(n+1, false);
    visited[i] = true;

    auto less = [&](const link_t &i, const link_t &j) { return i.cost > j.cost || (i.cost == j.cost && i.from_v > j.from_v); };
    std::priority_queue<link_t, std::vector<link_t>, decltype(less)> Q(less);

    for (long j = 1; j <= n; j++) if (j != i) Q.push({ i, j, c(i, j) });

    long number_of_disj_sets = n;
    long network_cost = 0;
    while (!Q.empty() && number_of_disj_sets > 1) { // if == 1 then graph is connected
        link_t new_link = Q.top();
        Q.pop();
        while (uf.find_set(new_link.new_v) == uf.find_set(i)) {
            if (Q.empty()) goto END;

            new_link = Q.top();
            Q.pop();
        }

        // new_link contains the link with less cost bringing to a new vertex
        uf.union_set(new_link.from_v, new_link.new_v);
        number_of_disj_sets--;

        assert(new_link.cost >= 0);
        network_cost += new_link.cost;
        for (long u = 1; u <= n; u++) if (visited[u]) hv(new_link.new_v, u) = std::max(new_link.cost, hv(new_link.from_v, u));
        am(new_link.from_v, new_link.new_v) = true;
        visited[new_link.new_v] = true;

        // also consider now the edges of the newly added vertex linking non-added vertices
        for (long j = 1; j <= n; j++) if (uf.find_set(j) != uf.find_set(new_link.new_v)) {
            Q.push({ new_link.new_v, j, c(new_link.new_v, j) });
        }
    }
    END:
    for (long u = 1; u <= n; u++) assert(visited[u]);
    return network_cost;
}


int main(int argc, const char * argv[]) {
    
    std::istream &fp = std::cin;
    //std::fstream fp("/Users/lore/Desktop/AlgoLab/AlgoLab/Week 10/week10/return_of_the_jedi/test1.in");
    
    int testcases;
    fp >> testcases;
    
    for (int t = 0; t < testcases; t++) {
        long n, i;
        fp >> n >> i;

        long *costs = new long[n*n];
        for (long j = 1; j < n; j++) {
            for (long k = 1; k <= n - j; k++) {
                fp >> c(j, j+k);
            }
        }

        std::vector<bool> _adj_m(n*n, false); // adjacency matrix of mst
        std::vector<long> _heaviest(n*n, -1); // (u, v) gives the heaviest edge between u and v in the mst
        long base_cost = leia_mst(costs, n, i, _adj_m, _heaviest);

        long second_cost = 1l << 30;
        for (long u = 1; u <= n; u++) for (long v = u+1; v <= n; v++) if (!am(u, v)) {
            long edge_to_add = c(u, v);
            long edge_to_remove = hv(u, v);

            second_cost = std::min(second_cost, base_cost + edge_to_add - edge_to_remove);
            if (second_cost == base_cost) break; 
        }

        std::cout << second_cost << "\n";
        delete[] costs;
    }
    
    return 0;
}
