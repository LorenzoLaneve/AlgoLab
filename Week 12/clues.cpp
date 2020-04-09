//
//  Clues
//  AlgoLab
//
//  Copyright © 2019 Lorenzo Laneve. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/nearest_neighbor_delaunay_2.h>

#include <boost/graph/adjacency_list.hpp>
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

bool is_two_colorable(graph &G, long n) {
    std::vector<long> coloring(n, 2);

    std::deque<std::pair<long, long>> Q;
    for (long s = 0; s < n; s++) {
        Q.push_back({ s, coloring[s] % 2 });

        while (!Q.empty()) {
            long v = Q.front().first;
            long d = Q.front().second;
            Q.pop_front();

            if (coloring[v] == 2) {
                coloring[v] = d % 2;

                graph::out_edge_iterator it, end;

                for (boost::tie(it, end) = boost::out_edges(v, G); it != end; it++) {
                    Q.push_back({ boost::target(*it, G), d+1 });
                }
            } else if (d % 2 != coloring[v]) { // check color
                return false;
            }
        }
    }

    return true;
}

void distance_bound_graph(graph &G, Triangulation &tr, boost::disjoint_sets_with_storage<> &uf, const std::vector<std::pair<K::Point_2, long>> &points, long r) {
    long k = 0;

    for (auto it = tr.finite_edges_begin(); it != tr.finite_edges_end(); it++) {
        auto u = it->first->vertex((it->second + 1) % 3);
        auto v = it->first->vertex((it->second + 2) % 3);

        if (CGAL::compare_squared_distance(points[u->info()].first, points[v->info()].first, r*r) != CGAL::LARGER) {
            boost::add_edge(u->info(), v->info(), G);
            uf.union_set(u->info(), v->info());
        }
    }

    long n = boost::num_vertices(G);

    std::vector<std::pair<long, long>> new_edges;
    std::vector<long> last_visit(n, -1);
    std::deque<long> Q;

    for (long s = 0; s < n; s++) {
        Q.push_back(s);
        last_visit[s] = s;

        while (!Q.empty()) {
            long v = Q.front();
            Q.pop_front();

            if (CGAL::compare_squared_distance(points[v].first, points[s].first, r*r) != CGAL::LARGER) {
                if (s < v) new_edges.push_back({ s, v });

                graph::out_edge_iterator it, end;
                for (boost::tie(it, end) = boost::out_edges(v, G); it != end; it++) {
                    long ng = boost::target(*it, G);
                    if (last_visit[ng] < s) {
                        Q.push_back(ng);
                        last_visit[ng] = s;
                    }
                }
            }
        }

        Q.clear();
    }

    for (auto &e : new_edges) boost::add_edge(e.first, e.second, G);
}

int main(int argc, const char * argv[]) {
    
    std::istream &fp = std::cin;
    //std::fstream fp("/Users/lore/Desktop/AlgoLab/AlgoLab/Week 12/week12/clues/test2.in");
    
    int testcases;
    fp >> testcases;
    
    for (int t = 0; t < testcases; t++) {
        long n, m, r;
        fp >> n >> m >> r;

        std::vector<std::pair<K::Point_2, long>> stations;
        for (long i = 0; i < n; i++) {
            long x, y;
            fp >> x >> y;

            stations.push_back({{ x, y }, i});
        }

        Triangulation tr(stations.begin(), stations.end());

        boost::disjoint_sets_with_storage<> uf(n);

        graph G(n);
        distance_bound_graph(G, tr, uf, stations, r);

        bool no_interference = is_two_colorable(G, n);
        
        for (long i = 0; i < m; i++) {
            long a_x, a_y, b_x, b_y;
            fp >> a_x >> a_y >> b_x >> b_y;

            if (!no_interference) {
                std::cout << "n";
                continue;
            }
            
            if (CGAL::compare_squared_distance(K::Point_2(a_x, a_y), K::Point_2(b_x, b_y), r*r) != CGAL::LARGER) {
                std::cout << "y";
                continue;
            }

            auto f_a = tr.nearest_vertex(K::Point_2(a_x, a_y));
            auto f_b = tr.nearest_vertex(K::Point_2(b_x, b_y));

            long first_station = f_a->info();
            long last_station = f_b->info();

            // yes <===> first and last are connected and a and b are close enough
            bool connected = uf.find_set(first_station) == uf.find_set(last_station);
            connected = connected && (CGAL::compare_squared_distance(K::Point_2(a_x, a_y), f_a->point(), r*r) != CGAL::LARGER);
            connected = connected && (CGAL::compare_squared_distance(f_b->point(), K::Point_2(b_x, b_y), r*r) != CGAL::LARGER);

            std::cout << (connected ? "y" : "n");
        }
        std::cout << "\n";
    }
    
    return 0;
}
