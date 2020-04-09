//
//  Search Snippets
//  AlgoLab
//
//  Copyright © 2019 Lorenzo Laneve. All rights reserved.
//

#include <iostream>
#include <queue>
#include <vector>
#include <cstring>

int main(int argc, const char *argv[]) {
    
    FILE *fp = stdin;//fopen("/Users/lore/Desktop/AlgoLab/AlgoLab/Week 2/week2/search_snippets/test3.in", "r");
    
    int testcases;
    fscanf(fp, "%d", &testcases);
    
    for (int t = 0; t < testcases; t++) {
        int n;
        fscanf(fp, "%d", &n);
        
        
        int m[n];
        int *p[n];
        for (int i = 0; i < n; i++) {
            int m_i;
            fscanf(fp, "%d", &m_i);
            
            m[i] = m_i;
            p[i] = new int[m_i];
        }
        
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m[i]; j++) {
                int p_ij;
                fscanf(fp, "%d", &p_ij);
                p[i][j] = p_ij;
            }
        }
        
        int word_index[n]; // index of the occurrence of the i-th word we are considering in the interval.
        memset(word_index, 0, sizeof(int)*n);
        
        auto more = [&](int L, int R) { return p[L][word_index[L]] >= p[R][word_index[R]]; };
        std::priority_queue<int, std::vector<int>, decltype(more)> min_heap(more);
        
        for (int i = 0; i < n; i++) {
            min_heap.push(i);
        }
        
        int first_word = min_heap.top();
        int last_word = 0;
        
        for (int i = 1; i < n; i++) {
            if (p[i][word_index[i]] > p[last_word][word_index[last_word]]) {
                last_word = i;
            }
        }
        
        
        int a = p[first_word][word_index[first_word]];
        int b = p[last_word][word_index[last_word]];
        while (word_index[first_word] + 1 < m[first_word]) {
            min_heap.pop();
            
            word_index[first_word]++;
            if (p[first_word][word_index[first_word]] > p[last_word][word_index[last_word]]) {
                last_word = first_word;
            }
            
            min_heap.push(first_word);
            
            first_word = min_heap.top();

            // check the new interval
            int new_a = p[first_word][word_index[first_word]];
            int new_b = p[last_word][word_index[last_word]];
            
            if (b - a > new_b - new_a) {
                a = new_a;
                b = new_b;
            }
        }
        
        printf("%d\n", b - a + 1);
        
        for (int i = 0; i < n; i++) delete[] p[i];
    }
    
    return 0;
}
