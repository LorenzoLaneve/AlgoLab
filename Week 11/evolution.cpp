//
//  Evolution
//  AlgoLab
//
//  Copyright © 2019 Lorenzo Laneve. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

struct species {
    std::string s;
    long a;
};

struct query {
    long index;

    long start;
    long age;

    long result;
};

void dfs(long root, std::vector<long> &path, const std::vector<species> &sp, const std::vector<std::vector<query *>> &qbs, const std::vector<std::vector<long>> &children) {
    path.push_back(root);

    for (query *q : qbs[root]) { // do queries starting from root
        // binary search
        long LB = 0, UB = path.size();

#define A(i) (sp[path[path.size() - (i) - 1]].a)

        while (LB < UB) {
            long mid = (LB + UB) / 2;

            if (A(mid) > q->age) {
                UB = mid;
            } else {
                LB = mid + 1;
            }
        }

        q->result = path[path.size() - (LB - 1) - 1];
    }

    for (long child : children[root]) { // go deeper
        dfs(child, path, sp, qbs, children);
    }

    path.pop_back();
}

int main(int argc, const char * argv[]) {
    
    std::istream &fp = std::cin;
    //std::fstream fp("/Users/lore/Desktop/AlgoLab/AlgoLab/Week 11/week11/evolution/sample.in");
    
    int testcases;
    fp >> testcases;
    
    for (int t = 0; t < testcases; t++) {
        long n, q;
        fp >> n >> q;

        std::map<std::string, long> name_index;

        std::vector<species> sp(n);
        std::vector<long> parent(n, -1); // i's parent, or -1 for luca
        std::vector<std::vector<long>> children(n); // i's children list

        for (int i = 0; i < n; i++) { 
            fp >> sp[i].s >> sp[i].a;
            name_index[sp[i].s] = i;
        }

        for (int i = 0; i < n - 1; i++) {
            std::string s, p;
            fp >> s >> p;

            parent[name_index[s]] = name_index[p];
            children[name_index[p]].push_back(name_index[s]);
        }

        std::vector<query> queries(q);
        std::vector<std::vector<query *>> q_by_species(n);
        for (int i = 0; i < q; i++) {
            std::string s;
            fp >> s >> queries[i].age;
            queries[i].start = name_index[s];
            queries[i].index = i;
            queries[i].result = -1;

            q_by_species[queries[i].start].push_back(&queries[i]);
        }

        long root = 0;
        for (int i = 0; i < n; i++) if (parent[i] < 0) {
            root = i;
            break;
        }

        std::vector<long> path;
        dfs(root, path, sp, q_by_species, children);

        long k = 0;
        for (query &q : queries) {
            if (k++ > 0) std::cout << " ";
            std::cout << sp[q.result].s;
        }

        std::cout << "\n";
    }
    
    return 0;
}
