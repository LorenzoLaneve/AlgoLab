//
//  Carsharing
//  AlgoLab
//
//  Copyright © 2019 Lorenzo Laneve. All rights reserved.
//

#include <iostream>
#include <fstream>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/find_flow_cost.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/cycle_canceling.hpp>

#define augment_edge(u, v, C, W) {\
        auto rev_e = boost::add_edge((v), (u), G);\
        \
        c_map[e.first] = (C);\
        c_map[rev_e.first] = 0;\
        r_map[rev_e.first] = e.first;\
        r_map[e.first] = rev_e.first;\
        w_map[e.first] = (W);\
        w_map[rev_e.first] = -(W);\
}

#define add_capacity_edge(u, v, C, W) {\
        auto e = boost::add_edge((u), (v), G);\
        augment_edge(u, v, C, W);\
    }

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
        boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
        boost::property<boost::edge_reverse_t, traits::edge_descriptor,
        boost::property<boost::edge_weight_t, long>>>>> graph;
typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

#define infinity (1000)

#define station_index(i, t) (s*(t)+ (i))

class vertex_table {
    graph &G;
    std::vector<long> vtb;
    long s;

public:
    vertex_table(graph &G, long s, long t) : G(G), vtb(s*t, -1), s(s) {  }

    bool exists(long i, long t) {
        return vtb[station_index(i, t)] >= 0;
    }

    long get(long i, long t) {
        long &ret = vtb[station_index(i, t)];
        if (ret < 0) ret = boost::add_vertex(G);
        return ret;
    };
};

struct request {
    long s, t, d, a, p;
    edge_desc e;
};

int main(int argc, const char * argv[]) {
    
    std::istream &fp = std::cin;
    //std::fstream fp("/Users/lore/Desktop/AlgoLab/AlgoLab/Week 13/week13/carsharing/test5.in");
    
    int testcases;
    fp >> testcases;
    
    for (int t = 0; t < testcases; t++) {
        long n, s;
        fp >> n >> s;

        graph G(2);
        auto c_map = boost::get(boost::edge_capacity, G);
        auto r_map = boost::get(boost::edge_reverse, G);
        auto w_map = boost::get(boost::edge_weight, G);

        const long source = 0;
        const long target = 1;

        std::vector<long> initial_cars(s);
        long number_of_cars = 0;
        for (long i = 0; i < s; i++) {
            fp >> initial_cars[i];
            number_of_cars += initial_cars[i];
        } 

        std::vector<request> requests(n);
        long max_time = 0;
        for (long i = 0; i < n; i++) {
            fp >> requests[i].s >> requests[i].t >> requests[i].d >> requests[i].a >> requests[i].p;
            max_time = std::max(max_time, requests[i].a);
        }
        
        vertex_table vtb(G, s, max_time+1);

        for (long i = 0; i < s; i++) add_capacity_edge(source, vtb.get(i, 0), initial_cars[i], 0);

        for (request &rq : requests) {
            long u = vtb.get(rq.s-1, rq.d); // s and t are 1-based!
            long v = vtb.get(rq.t-1, rq.a);

            auto e = boost::add_edge(u, v, G);
            augment_edge(u, v, 1, 100*(rq.a - rq.d) - rq.p);
            rq.e = e.first;
        }

        for (long i = 0; i < s; i++) {
            long last_time = 0;
            for (long time = 1; time <= max_time; time++) if (vtb.exists(i, time)) {
                add_capacity_edge(vtb.get(i, last_time), vtb.get(i, time), 1000, 100*(time - last_time));
                last_time = time;
            }

            add_capacity_edge(vtb.get(i, last_time), target, 1000, 100*(max_time - last_time));
        }

        auto rc_map = boost::get(boost::edge_residual_capacity, G);
        boost::successive_shortest_path_nonnegative_weights(G, source, target);
        
        long profit = 0;
        for (request &rq : requests) if (!rc_map[rq.e]) profit += rq.p;

        std::cout << profit << "\n";
    }
    
    return 0;
}
