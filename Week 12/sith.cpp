//
//  Revenge of the Sith
//  AlgoLab
//
//  Copyright © 2019 Lorenzo Laneve. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/nearest_neighbor_delaunay_2.h>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

#include <boost/pending/disjoint_sets.hpp>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

typedef CGAL::Triangulation_vertex_base_with_info_2<long, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Triangulation;
typedef Triangulation::Finite_faces_iterator Face_iterator;

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::undirectedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::no_property, boost::no_property> graph;
typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

int main(int argc, const char * argv[]) {
    
    std::istream &fp = std::cin;
    //std::fstream fp("/Users/lore/Desktop/AlgoLab/AlgoLab/Week 12/week12/sith/test1.in");
    
    int testcases;
    fp >> testcases;
    
    for (int t = 0; t < testcases; t++) {
        long n, r;
        fp >> n >> r;

        std::vector<K::Point_2> planets;
        for (long i = 0; i < n; i++) {
            long x, y; fp >> x >> y;
            planets.push_back({ x, y });
        }

        // visit planets in reverse order and add them to a triangulation. then check if they can be linked to an existing planet
        // (i.e. the nearest neighbor is <= r). Then use union find to track connected components.

        Triangulation tr;
        boost::disjoint_sets_with_storage<> uf(n);
        std::vector<long> set_sizes(n, 1); // size of set represented by i

        long max_cc_size = 1;
        for (long i = n - 1; i >= 0; i--) { // from last day
            K::Point_2 &p = planets[i];
            auto new_v = tr.insert(p);
            new_v->info() = i;

            if (tr.number_of_vertices() <= 1) continue;

            Triangulation::Vertex_circulator it, start;
            it = start = new_v->incident_vertices();
            do {
                if (!tr.is_infinite(it) && CGAL::compare_squared_distance(new_v->point(), it->point(), r*r) != CGAL::LARGER) {
                    // the two vertices are in the same connected component at day i

                    if (uf.find_set(new_v->info()) == uf.find_set(it->info())) continue;
                    // sizes should not be updated if union find doesn't change!

                    long u_size = set_sizes[uf.find_set(new_v->info())];
                    long v_size = set_sizes[uf.find_set(it->info())];
                    uf.union_set(new_v->info(), it->info());

                    long new_size = u_size + v_size;

                    set_sizes[uf.find_set(new_v->info())] = new_size;
                    max_cc_size = std::max(max_cc_size, std::min(i, new_size));
                    // if i days are passed, only i planets of the component can be retrieved.
                }
            } while (++it != start);
        }

        std::cout << max_cc_size << "\n";
    }
    
    return 0;
}
