//
//  Goldeneye
//  AlgoLab
//
//  Copyright © 2019 Lorenzo Laneve. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <algorithm>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>

#include <boost/pending/disjoint_sets.hpp>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<long, K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Triangulation;
typedef Triangulation::Finite_faces_iterator Face_iterator;

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::undirectedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::no_property,
    boost::property<boost::edge_weight_t, K::FT>> graph;
typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;


long ceil_to_int(K::FT x) {
    double a = CGAL::to_double(x);
    while (a < x) a += 1;
    while (a-1 >= x) a -= 1;
    return (long) a;
}

struct triangulation_edge {
    long u;
    long v;
    K::FT distance;
};

int main(int argc, const char * argv[]) {
    
    std::istream &fp = std::cin;
    //std::fstream fp("/Users/lore/Desktop/AlgoLab/AlgoLab/Week 9/week9/goldeneye/test1.in");
    
    int testcases;
    fp >> testcases;
    
    for (int t = 0; t < testcases; t++) {
        long n, m, p;
        fp >> n >> m >> p;

        std::vector<std::pair<K::Point_2, long>> jammers;
        for (int i = 0; i < n; i++) {
            long x, y;
            fp >> x >> y;

            jammers.push_back({{ x, y }, i });
        }

        Triangulation tr(jammers.begin(), jammers.end());

        graph G(n);
        auto w_map = boost::get(boost::edge_weight, G);

        std::vector<triangulation_edge> edges;
        for (auto it = tr.finite_edges_begin(); it != tr.finite_edges_end(); it++) {
            long u = it->first->vertex((it->second+1) % 3)->info();
            long v = it->first->vertex((it->second+2) % 3)->info();

            K::FT distance = tr.segment(it).squared_length();

            edges.push_back({ u, v, distance });
        }

        std::sort(edges.begin(), edges.end(), [&](const triangulation_edge &i, const triangulation_edge &j) {
            return i.distance < j.distance;
        });

        boost::disjoint_sets_with_storage<> uf(n);
        boost::disjoint_sets_with_storage<> ua(n);
        boost::disjoint_sets_with_storage<> ub(n);
        for (triangulation_edge &e : edges) {
            if (e.distance > p) break; 

            uf.link(e.u, e.v);
        }

        long last_for_a = 0;
        long last_for_b = 0;
        K::FT a, b;
        a = b = 0;
        for (int i = 0; i < m; i++) {
            long s_x, s_y, t_x, t_y;
            fp >> s_x >> s_y >> t_x >> t_y;

            auto s_nearest_jammer = tr.nearest_vertex({ s_x, s_y });
            auto t_nearest_jammer = tr.nearest_vertex({ t_x, t_y });

            K::FT d_s = 4*CGAL::squared_distance(s_nearest_jammer->point(), { s_x, s_y });
            K::FT d_t = 4*CGAL::squared_distance(t_nearest_jammer->point(), { t_x, t_y });

            if (d_s <= p && d_t <= p &&
                uf.find_set(s_nearest_jammer->info()) == uf.find_set(t_nearest_jammer->info())) {
                std::cout << "y";

                b = std::max(b, std::max(d_s, d_t));
                while (ub.find_set(s_nearest_jammer->info()) != ub.find_set(t_nearest_jammer->info())) {
                    ub.link(edges[last_for_b].u, edges[last_for_b].v);
                    b = std::max(b, edges[last_for_b++].distance);
                } 
            } else {
                std::cout << "n";
            }

            a = std::max(a, std::max(d_s, d_t));
            while (ua.find_set(s_nearest_jammer->info()) != ua.find_set(t_nearest_jammer->info())) {
                ua.link(edges[last_for_a].u, edges[last_for_a].v);
                a = std::max(a, edges[last_for_a++].distance);
            } 
        }
        std::cout << "\n" << ceil_to_int(a) << "\n";
        std::cout << ceil_to_int(b) << "\n";

    }
    
    return 0;
}
