//
//  H1N1
//  AlgoLab
//
//  Copyright © 2019 Lorenzo Laneve. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <queue>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<long, K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Triangulation;
typedef Triangulation::Finite_faces_iterator Face_iterator;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
boost::no_property, boost::property<boost::edge_weight_t, K::FT> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;


int main(int argc, const char * argv[]) {
    
    std::istream &fp = std::cin;
    //std::fstream fp("/Users/lore/Desktop/AlgoLab/AlgoLab/Week 8/week8/h1n1/h1n1_test.in");
    
    int n, m;
    while (fp >> n && n != 0) {
        std::vector<K::Point_2> infected_points;
        
        for (int i = 0; i < n; i++) {
            long x, y;
            fp >> x >> y;
            infected_points.push_back({ x, y });
        }
        Triangulation t(infected_points.begin(), infected_points.end());
        
        auto end = t.finite_faces_end();
        
        
        long index = 0;
        for (auto it = t.finite_faces_begin(); it != end; it++) it->info() = index++;
        
        weighted_graph G(index + 1);
        auto w_map = boost::get(boost::edge_weight, G);
        
        const long infinite_vertex = index;
        
        for (auto it = t.finite_faces_begin(); it != end; it++) {
            for (int neigh = 0; neigh < 3; neigh++) {
                auto neigh_face = it->neighbor(neigh);
                
                long neigh_vertex;
                if (t.is_infinite(neigh_face)) {
                    neigh_vertex = infinite_vertex;
                } else {
                    neigh_vertex = neigh_face->info();
                }
                
                auto neigh_v1 = it->vertex((neigh + 1) % 3)->point();
                auto neigh_v2 = it->vertex((neigh + 2) % 3)->point();
                
                K::Segment_2 seg(neigh_v1, neigh_v2);
                
                auto edge = boost::add_edge(it->info(), neigh_vertex, G).first;
                w_map[edge] = seg.squared_length();
                
            }
        }
        
        typedef enum { BLACK, RED } v_color;
        
        std::vector<K::FT> distances(index + 1);
        std::vector<v_color> colors(index + 1);
        for (int i = 0; i <= index; i++) distances[i] = 0;
        for (int i = 0; i <= index; i++) colors[i] = BLACK;
        
        distances[infinite_vertex] = 1l << 50;
        
        auto less = [&](const std::pair<K::FT, long> u, const std::pair<K::FT, long> v) { return u.first < v.first; };
        std::priority_queue<std::pair<K::FT, long>, std::vector<std::pair<K::FT, long>>, decltype(less)> vertices(less);
        vertices.push({ 1l << 50, infinite_vertex });
        while (!vertices.empty()) {
            auto vertex = vertices.top();
            vertices.pop();
            
            if (colors[vertex.second] == BLACK) {
                weighted_graph::out_edge_iterator it, end;
                for (boost::tie(it, end) = boost::out_edges(vertex.second, G); it != end; it++) {
                    long outer = boost::target(*it, G);
                    
                    if (distances[outer] < CGAL::min(distances[vertex.second], w_map[*it])) {
                        distances[outer] = CGAL::min(distances[vertex.second], w_map[*it]);
                        vertices.push({ distances[outer], outer });
                    }
                }
                
                colors[vertex.second] = RED;
            }
        }
        
        
        fp >> m;
        for (int i = 0; i < m; i++) {
            long x, y, d;
            fp >> x >> y >> d;
            
            K::Point_2 escaping_point = { x, y };
            
            if (CGAL::compare_squared_distance(escaping_point, t.nearest_vertex(escaping_point)->point(), d) != CGAL::SMALLER) {
                auto face = t.locate({ x, y });
                if (t.is_infinite(face) || distances[face->info()] >= 4*d) {
                    std::cout << "y";
                } else {
                    std::cout << "n";
                }
            } else {
                std::cout << "n";
            }
        }
        
        std::cout << "\n";
    }
    
    return 0;
}
