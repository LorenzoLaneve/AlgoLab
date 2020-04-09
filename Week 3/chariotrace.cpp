//
//  Chariot Race
//  AlgoLab
//
//  Copyright © 2019 Lorenzo Laneve. All rights reserved.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>

#define memo(i, b) (_memo[2*i+(b?1:0)])
#define childcosts(i, b) (_child_costs[2*i+(b?1:0)])

#define recurse(i, b) (min_cost(children, parent, c, (i), (b)))


#define min3(a, b, c) std::min((a), std::min((b), (c)));



long min_cost(std::vector<std::vector<int>> &children, int *parent, long *c, int i, bool parent_repaired);

long *_child_costs;

long child_costs(std::vector<std::vector<int>> &children, int *parent, long *c, int i, bool repaired) {
    if (childcosts(i, repaired) >= 0) {
        return childcosts(i, repaired);
    }
    
    long child_costs = 0; // costs of children when I repair i.
    for (int child : children[i]) {
        child_costs += recurse(child, repaired);
    }
    
    return childcosts(i, repaired) = child_costs;
}


long *_memo;

long min_cost(std::vector<std::vector<int>> &children, int *parent, long *c, int i, bool parent_repaired) {
    if (memo(i, parent_repaired) >= 0) {
        return memo(i, parent_repaired);
    }
    
    if (children.empty()) {
        if (parent_repaired) {
            return memo(i, true) = 0;
        } else {
            return memo(i, false) = c[i];
        }
    }
    
    
    
    long child_costs_repaired = child_costs(children, parent, c, i, true);

    
    
    long child_costs_unrepaired = child_costs(children, parent, c, i, false);
    
    long subchild_costs = std::numeric_limits<long>::max();
    for (int child : children[i]) {
        long subchild_candidate = c[child] + child_costs(children, parent, c, child, true);
        
        subchild_candidate += child_costs_unrepaired - recurse(child, false);
        
        if (subchild_costs > subchild_candidate) {
            subchild_costs = subchild_candidate;
        }
    }
    
    if (!parent_repaired) {
        return memo(i, parent_repaired) = std::min(c[i] + child_costs_repaired, subchild_costs);
    }

    return memo(i, parent_repaired) = min3(
                                           c[i] + child_costs_repaired,
                                           child_costs_unrepaired,
                                           subchild_costs
                                           );
}



int main(int argc, const char * argv[]) {
    
    FILE *fp = stdin;//fopen("/Users/lore/Desktop/AlgoLab/AlgoLab/Week 3/week3/chariot-race/sample.in", "r");
    
    int testcases;
    fscanf(fp, "%d", &testcases);
    
    for (int t = 0; t < testcases; t++) {
        int n;
        fscanf(fp, "%d", &n);
        
        std::vector<std::vector<int>> children(n);
        int parent[n];
        parent[0] = -1;
        for (int i = 1; i < n; i++) {
            int a, b;
            fscanf(fp, "%d %d", &a, &b);
            
            children[a].push_back(b);
            parent[b] = a;
        }
        
        long c[n];
        for (int i = 0; i < n; i++) {
            fscanf(fp, "%ld", &c[i]);
        }
        
        
        _memo = new long[2*n];
        _child_costs = new long[2*n];
        
        for (int i = 0; i < 2*n; i++) {
            _memo[i] = -1;
            _child_costs[i] = -1;
        }
        
        
        printf("%ld\n", min_cost(children, parent, c, 0, false));
        
        delete [] _memo;
        delete [] _child_costs;
    }
    
    return 0;
}
