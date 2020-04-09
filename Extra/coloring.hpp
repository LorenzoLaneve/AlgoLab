
#ifndef __two_coloring
#define __two_coloring

#include <queue>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

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

#endif