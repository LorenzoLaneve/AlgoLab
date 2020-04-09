//
//  London
//  AlgoLab
//
//  Copyright © 2019 Lorenzo Laneve. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>


#define add_unit_capacity_edge(u, v) {\
    auto e = boost::add_edge((u), (v), G);\
    auto rev_e = boost::add_edge((v), (u), G);\
    \
    c_map[e.first] = 1;\
    c_map[rev_e.first] = 0;\
    r_map[rev_e.first] = e.first;\
    r_map[e.first] = rev_e.first;\
}

#define add_capacity_edge(u, v, C) {\
    auto e = boost::add_edge((u), (v), G);\
    auto rev_e = boost::add_edge((v), (u), G);\
    \
    c_map[e.first] = (C);\
    c_map[rev_e.first] = 0;\
    r_map[rev_e.first] = e.first;\
    r_map[e.first] = rev_e.first;\
}

#define alphabet_length ('Z' - 'A' + 1)


typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
boost::property<boost::edge_capacity_t, long,
boost::property<boost::edge_residual_capacity_t, long,
boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;
typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

#define front_char(row, col) front[w*row + col]
#define back_char(row, col) back[w*row + col]

#define letter_vertex(i) ((i) - 'A')

#define occs_in_note(i) letters_count[(i) - 'A']
#define occs_in_cells(i, j) cell_types[alphabet_length*((i) - 'A') + (j) - 'A']

int main(int argc, const char * argv[]) {
    std::ios_base::sync_with_stdio(false);
    
    //std::ifstream fp("/Users/lore/Desktop/AlgoLab/AlgoLab/Week 6/week6/london/test5.in");
    std::istream &fp = std::cin;
    
    int testcases;
    fp >> testcases;
    
    for (int t = 0; t < testcases; t++) {
        long h, w;
        fp >> h >> w;
        
        std::string note;
        fp >> note;
        
        char *front = new char[h * w];
        char *back = new char[h * w];
        
        for (int i = 0; i < h; i++) {
            char line_buffer[w+1];
            fp >> line_buffer;
            
            char *dest = &front_char(i, 0);
            char *src = line_buffer;
            for (long i = 0; i < w; i++) *dest++ = *src++;
        }
        
        for (int i = 0; i < h; i++) {
            char line_buffer[w+1];
            fp >> line_buffer;
            
            char *dest = &back_char(i, w-1);
            char *src = line_buffer;
            for (long i = 0; i < w; i++) *dest-- = *src++;
        }
        
        
        long letters_count[alphabet_length];
        memset(letters_count, 0, sizeof(long)*alphabet_length);
        for (long i = 0; i < note.size(); i++) letters_count[note[i] - 'A']++;
        
        graph G(alphabet_length+2);
        auto c_map = boost::get(boost::edge_capacity, G);
        auto r_map = boost::get(boost::edge_reverse, G);
        
        const long source = alphabet_length;
        const long target = alphabet_length+1;
        
        for (int letter = 'A'; letter <= 'Z'; letter++) {
            add_capacity_edge(letter_vertex(letter), target, occs_in_note(letter));
        }
        
        long cell_types[alphabet_length*alphabet_length];
        memset(cell_types, 0, sizeof(long)*alphabet_length*alphabet_length);
        
        for (int row = 0; row < h; row++) {
            for (int col = 0; col < w; col++) {
                if (occs_in_note(front_char(row, col)) > 0 || occs_in_note(back_char(row, col)) > 0) {
                    int i = front_char(row, col);
                    int j = back_char(row, col);
                    
                    if (i > j) std::swap(i, j);
                    
                    occs_in_cells(i, j)++;
                }
            }
        }
        
        for (int i = 'A'; i <= 'Z'; i++) {
            for (int j = i; j <= 'Z'; j++) {
                // i <= j
                if (occs_in_cells(i, j) > 0) {
                    vertex_desc cell_vertex = boost::add_vertex(G);
                    
                    add_capacity_edge(source, cell_vertex, occs_in_cells(i, j));
                    
                    add_capacity_edge(cell_vertex, letter_vertex(i), occs_in_cells(i, j));
                    
                    if (i != j) {
                        add_capacity_edge(cell_vertex, letter_vertex(j), occs_in_cells(i, j));
                    }
                }
            }
        }
        
        
        long max_flow = boost::push_relabel_max_flow(G, source, target);
        
        if (max_flow >= note.size()) {
            std::cout << "Yes\n";
        } else {
            std::cout << "No\n";
        }
        
        delete [] front;
        delete [] back;
    }
    
    return 0;
}
