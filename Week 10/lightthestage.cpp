//
//  Light the Stage
//  AlgoLab
//
//  Copyright © 2019 Lorenzo Laneve. All rights reserved.
//

#include <iostream>
#include <fstream>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>

#include <boost/pending/disjoint_sets.hpp>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Triangulation::Finite_faces_iterator Face_iterator;



struct person {
    long index;

    K::Point_2 location;
    long radius;
};

int main(int argc, const char * argv[]) {
    
    //std::istream &fp = std::cin;
    std::fstream fp("/Users/lore/Desktop/AlgoLab/AlgoLab/Week 10/week10/light/light-test1.in");
    
    int testcases;
    fp >> testcases;
    
    for (int t = 0; t < testcases; t++) {
        int n, m;
        fp >> m >> n;

        std::vector<person> people;
        std::vector<long> disq_round(m, n);

        for (int i = 0; i < m; i++) {
            long x, y, r;
            fp >> x >> y >> r;

            people.push_back({ i, { x, y }, r });
        }

        long h; fp >> h; // l_r = h*tan(45) = h!

        std::vector<K::Point_2> lights;
        for (int i = 0; i < n; i++) {
            long x, y; fp >> x >> y;
            lights.push_back({ x, y });
        }

        Triangulation tr(lights.begin(), lights.end());
        int i;
        for (i = 0; i < m; i++) {
            const person &p = people[i];

            K::FT dist = h + p.radius;

            for (long j = 0; j < n; j++) {
                if (CGAL::squared_distance(p.location, lights[j]) < dist*dist) {
                    disq_round[i] = j;
                    break;
                }
            }

            if (disq_round[i] == n) { i++; break; }
        }

        for (; i < m; i++) {
            const person &p = people[i];

            auto nearest_vertex = tr.nearest_vertex(p.location);
            auto nearest_light = nearest_vertex->point();

            K::FT dist = h + p.radius;

            // |l - p| < h + r_p <==> positive area
            if (CGAL::squared_distance(p.location, nearest_light) < dist*dist) {
                disq_round[i] = 0;
            }
        }

        long winning_round = 0;
        for (int i = 0; i < m; i++) {
            if (winning_round < disq_round[i]) winning_round = disq_round[i];
        }

        for (int i = 0; i < m; i++) if (disq_round[i] == winning_round) {
            std::cout << i << " ";
        }
        std::cout << "\n";

    }
    
    return 0;
}