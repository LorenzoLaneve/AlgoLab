//
//  Buddy Selection
//  AlgoLab
//
//  Copyright © 2019 Lorenzo Laneve. All rights reserved.
//

#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
boost::no_property, boost::no_property >                                weighted_graph;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;


typedef std::pair<unsigned long, unsigned long> interest_ID;

// 20 chars lowercase letters 26 => 5 bits per char. 5*20 = 120 bit
// => 128-bit hash is enough
interest_ID hash(const char *str) {
    
    unsigned long x = 0;
    unsigned long y = 0;
    
    bool str_is_over = false;
    for (int i = 0; i < 10; i++) {
        if (!str[i]) str_is_over = true;
        
        x <<= 5;
        if (!str_is_over) x |= (str[i] - 'a');
    }
    
    for (int i = 10; i < 20; i++) {
        if (!str[i]) str_is_over = true;

        y <<= 5;
        if (!str_is_over) y |= (str[i] - 'a');
    }
    
    return interest_ID(x, y);
}

inline bool compare_hash(const interest_ID &x, const interest_ID &y) {
    return x.first == y.first && x.second == y.second;
}

inline bool hash_less_equal(const interest_ID &x, const interest_ID &y) {
    return x.first <= y.first || (x.first == y.first && x.second <= y.second);
}



int main(int argc, const char *argv[]) {
    
    FILE *fp = stdin;//fopen("/Users/lore/Desktop/AlgoLab/AlgoLab/Week 4/week4/buddy_selection/test3.in", "r");
    
    int testcases;
    fscanf(fp, "%d", &testcases);
    
    for (int t = 0; t < testcases; t++) {
        int n, c, f;
        fscanf(fp, "%d %d %d", &n, &c, &f);
        
        std::vector<std::vector<interest_ID>> people_interests(n);
        
        weighted_graph G(n);
        
        std::vector<std::pair<int, int>> edges;
        
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < c; j++) {
                char str_data[21];
                fscanf(fp, "%s", str_data);
                
                people_interests[i].push_back(hash(str_data));
            }
        }
        
        for (int i = 0; i < n; i++) {
            std::stable_sort(people_interests[i].begin(), people_interests[i].end(), hash_less_equal);
        }
        
        int affinity = 0;
        for (int i = 0; i < n; i++) {
            for (int j = i+1; j < n; j++) {
                // affinity between i and j
                affinity = 0;
                
                int p, q;
                p = q = 0;
                while (p < c && q < c) {
                    if (hash_less_equal(people_interests[i][p], people_interests[j][q])) {
                        if (compare_hash(people_interests[i][p], people_interests[j][q])) {
                            if (++affinity > f) {
                                boost::add_edge(i, j, G);
                                break;
                            }
                            p++;
                            q++;
                        } else p++;
                    } else {
                        q++;
                    }
                }
            }
        }
        
        
        std::vector<vertex_desc> mate_map(n); // exterior property map
        const vertex_desc NULL_VERTEX = boost::graph_traits<weighted_graph>::null_vertex();
        
        boost::edmonds_maximum_cardinality_matching(G, boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));
        
        bool no_one_is_alone = true;
        for (vertex_desc v : mate_map) {
            if (v == NULL_VERTEX) {
                no_one_is_alone = false;
                break;
            }
        }
        
        
        if (no_one_is_alone) {
            printf("not optimal\n");
        } else {
            printf("optimal\n");
        }
    }
    
    return 0;
}
