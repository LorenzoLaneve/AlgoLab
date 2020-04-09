//
//  New York
//  AlgoLab
//
//  Copyright © 2019 Lorenzo Laneve. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <functional>
#include <vector>
#include <queue>
#include <stack>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/strong_components.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
            boost::no_property, boost::no_property>      graph;
typedef boost::graph_traits<graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<graph>::vertex_descriptor          vertex_desc;

typedef std::priority_queue<long, std::vector<long>, std::function<bool(long, long)>> heap;


#define is_in_window(v) (positions[v] >= 0 && window.size() - m <= positions[v] && positions[v] < window.size())


typedef enum { VISIT, REMOVE } cmd_type;

typedef struct {
    cmd_type cmd;
    long v;
} dfs_command;

void dfs_iterative(long n, long m, long k, std::vector<bool> &eligible, const std::vector<long> &h, const graph &G) {
    std::stack<dfs_command> dfs_stack;
    dfs_stack.push({ VISIT, 0l });

    auto colder = [&](const long i, const long j) { return h[i] > h[j]; };
    auto hotter = [&](const long i, const long j) { return h[i] < h[j]; };
    heap coldest(colder);
    heap hottest(hotter);

    std::vector<long> window;
    std::vector<long> positions(n, -1);

    while (!dfs_stack.empty()) {
        auto c = dfs_stack.top();
        dfs_stack.pop();

        long v = c.v;
        if (c.cmd == VISIT) {
            dfs_stack.push({ REMOVE, v });

            window.push_back(v);
            positions[v] = window.size() - 1;

            hottest.push(v);
            coldest.push(v);

            // do not remove elements from window, just consider the last m elements.
            //std::cout << v << ": ";
            if (window.size() >= m) {
                while (!is_in_window(hottest.top())) hottest.pop(); // O(n log n) amortized over the whole algorithm!
                while (!is_in_window(coldest.top())) coldest.pop(); // since we add at most 2n vertices

                // both hottest and coldest are in the current window
                //std::cout << "(" << coldest.top() << "," << hottest.top() << ")\n";

                if (h[hottest.top()] - h[coldest.top()] <= k) eligible[window[window.size() - m]] = true;
            }

            graph::out_edge_iterator it, end;
            for (boost::tie(it, end) = boost::out_edges(v, G); it != end; it++) {
                long u = boost::target(*it, G);
                dfs_stack.push({ VISIT, u });
            }
        } else { // remove
            positions[c.v] = -1;
            window.pop_back();

            if (window.size() >= m) { // we need to add back the first vertex to the heaps
                hottest.push(window[window.size() - m]);
                coldest.push(window[window.size() - m]);
            }
        }
        
    }
}

void dfs_recursive(long v, long m, long k, std::vector<long> &window, std::vector<long> &positions, heap &hottest, heap &coldest, std::vector<bool> &eligible, const std::vector<long> &h, const graph &G) {
    window.push_back(v);
    positions[v] = window.size() - 1;

    hottest.push(v);
    coldest.push(v);

    // do not remove elements from window, just consider the last m elements.
    //std::cout << v << ": ";
    if (window.size() >= m) {
        while (!is_in_window(hottest.top())) hottest.pop(); // O(n log n) amortized over the whole algorithm!
        while (!is_in_window(coldest.top())) coldest.pop(); // since we add at most 2n vertices

        // both hottest and coldest are in the current window
        //std::cout << "(" << coldest.top() << "," << hottest.top() << ")\n";

        if (h[hottest.top()] - h[coldest.top()] <= k) eligible[window[window.size() - m]] = true;
    }

    graph::out_edge_iterator it, end;
    for (boost::tie(it, end) = boost::out_edges(v, G); it != end; it++) {
        long u = boost::target(*it, G);
        dfs_recursive(u, m, k, window, positions, hottest, coldest, eligible, h, G);
    }

    positions[v] = -1;
    window.pop_back();

    if (window.size() >= m) { // we need to add back the first vertex to the heaps
        hottest.push(window[window.size() - m]);
        coldest.push(window[window.size() - m]);
    }
}



int main(int argc, const char * argv[]) {
    std::cin.sync_with_stdio(false);
    std::istream &fp = std::cin;
    //std::fstream fp("/Users/lore/Desktop/AlgoLab/AlgoLab/Week 13/week13/new_york/test4.in");
    
    int testcases;
    fp >> testcases;
    
    for (int t = 0; t < testcases; t++) {
        long n, m, k;
        fp >> n >> m >> k;

        std::vector<long> h(n);
        for (long i = 0; i < n; i++) fp >> h[i];

        graph G(n);
        for (long i = 1; i < n; i++) {
            long u, v; fp >> u >> v;
            boost::add_edge(u, v, G);
        }

        // exactly one trip from NY to s ==> G is a tree!
        std::vector<bool> eligible(n, false);

#ifdef RECURSIVE_DFS
        auto colder = [&](const long i, const long j) { return h[i] > h[j]; };
        auto hotter = [&](const long i, const long j) { return h[i] < h[j]; };
        heap coldest(colder);
        heap hottest(hotter);

        std::vector<long> window;
        std::vector<long> positions(n, -1);
        dfs_recursive(0, m, k, window, positions, hottest, coldest, eligible, h, G);
#else
        dfs_iterative(n, m, k, eligible, h, G);
#endif

        bool no_eligibles = true;
        for (long i = 0; i < n; i++) if (eligible[i]) {
            std::cout << i << " ";
            no_eligibles = false;
        }

        if (no_eligibles) std::cout << "Abort mission";
        std::cout << "\n";
    }
    
    return 0;
}
