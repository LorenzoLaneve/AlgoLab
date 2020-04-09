#ifndef __distance_bound_graph
#define __distance_bound_graph

#include <vector>
#include <queue>

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

void distance_bound_graph(graph &G, const std::vector<K::Point_2> &points, long r) {
    Triangulation tr(points.begin(), points.end());

    long k = 0;
    for (auto it = tr.finite_vertices_begin(); it != tr.finite_vertices_end(); it++) it->info() = k++;

    for (auto it = tr.finite_edges_begin(); it != tr.finite_edges_end(); it++) {
        auto u = it.first->vertex((it->second + 1) % 3);
        auto v = it.first->vertex((it->second + 2) % 3);

        if (CGAL::compare_squared_distance(stations[i].first, stations[u].first, r*r) != CGAL::LARGER) {
            boost::add_edge(G, u->info(), v->info());
        }
    }

    std::vector<std::pair<long, long>> new_edges;
    std::vector<long> last_visit(n, -1);
    std::queue<long> Q;


    for (long s = 0; s < n - 1; s++) {
        Q.push_back(s);
        last_visit[s] = s;

        while (!Q.empty()) {
            long v = Q.front();
            Q.pop_front();

            if (CGAL::compare_squared_distance(points[v], points[s], r*r) != CGAL::LARGER) {
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

#endif