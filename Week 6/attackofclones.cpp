//
//  Attack of the Clones
//  AlgoLab
//
//  Copyright © 2019 Lorenzo Laneve. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
#include <algorithm>

struct interval {
    long a;
    long b;
};

struct event {
    long diff;
    long p;
};

#define len(a, b) ((a) <= (b) ? ((b) - (a)) : (m + (b) - (a)) % m)

#define interval_covers_segment(a, b, i) ((a) <= (b) ? ((a) <= (i) && (i) <= (b)) : ((a) <= (i) || (i) <= (b)))



long greedy_interval_scheduling(long start, long first_free_segment, long last_free_segment, long n, long m, const std::vector<interval> &intervals) {
    std::vector<interval> taken_intervals;

    for (const interval &it : intervals) {
        if (it.b > last_free_segment) break;

        if (first_free_segment <= it.a) {
            first_free_segment = it.b + 1;
            taken_intervals.push_back({ it.a + m, it.b + m });
        }
    }

    return taken_intervals.size();
}

long greedy_interval_sch_start(long start, long n, long m, const std::vector<interval> &intervals) {
    long first_free_segment = intervals[start].b + 1;
    long last_free_segment = intervals[start].a + m - 1;
    return greedy_interval_scheduling(start+1, first_free_segment, last_free_segment, n, m, intervals) + 1;
    // +1 because start is already taken
}

long greedy_interval_no_start(long free_segment, long n, long m, const std::vector<interval> &intervals) {
    long first_free_segment = free_segment + 1;
    long last_free_segment = free_segment + m - 1;

    //for (const interval &it : intervals) std::cout << "(" << (it.a) << ", " << (it.b) << ") ";

    return greedy_interval_scheduling(0, first_free_segment, last_free_segment, n, m, intervals);
}

int main(int argc, const char * argv[]) {

    std::cin.sync_with_stdio(false);
    std::istream &fp = std::cin;
    //std::fstream fp("/Users/lore/Desktop/AlgoLab/AlgoLab/Week 6/week6/attack_of_the_clones/testsets/test1.in");
    
    int testcases;
    fp >> testcases;
    
    for (int t = 0; t < testcases; t++) {
        long n, m;
        fp >> n >> m;
        
        std::vector<interval> intervals;
        std::vector<event> raw_events;

        long current_intervals = 0;
        for (long i = 0; i < n; i++) {
            long a_i, b_i;
            fp >> a_i >> b_i;
            a_i -= 1;
            b_i -= 1; // { 1, ..., m } -> { 0, ..., m - 1 }

            if (b_i < a_i) {
                current_intervals++; // it contains segment 0 
                intervals.push_back({ a_i, b_i + m });
            } else {
                intervals.push_back({ a_i, b_i });
            }

            raw_events.push_back({ +1, a_i });
            raw_events.push_back({ -1, b_i+1 }); // from the successive segment there will be -1
        }
 
        std::sort(raw_events.begin(), raw_events.end(), [&](const event &i, const event &j) {
            return i.p < j.p;
        });

        // compress events, one for each segment
        long k = 0;
        std::vector<event> events;
        while (k < raw_events.size()) {
            event new_e = { 0, raw_events[k].p };

            while (k < raw_events.size() && raw_events[k].p == new_e.p) {
                new_e.diff += raw_events[k++].diff;
            }

            events.push_back(new_e);
        }

        // starting from 0 (current_intervals)
        long current_segment = 0;
        k = 0;
        while (current_intervals > 10) { // k < size() certainly
            event &e = events[k++];
            current_segment = e.p;
            current_intervals += e.diff;
        }

        // current_segment is covered by at most 10 intervals!
        std::vector<interval> starting_intervals;
        for (long i = 0; i < n; i++) {
            const interval &it = intervals[i];
            if (interval_covers_segment(it.a % m, it.b % m, current_segment)) {
                starting_intervals.push_back(it);
            }
        }

        // starting_intervals.size() <= 10 certainly
        for (long i = 0; i < n; i++) intervals.push_back({ intervals[i].a + m, intervals[i].b + m });

        std::sort(intervals.begin(), intervals.end(), [&](const interval &i, const interval &j) {
             return i.b < j.b;
        });
        
        long max_intervals = 0;
        for (const interval &it : starting_intervals) {
            long start = 0; // linear search on starting intervals since they changed index
            for (; start < intervals.size(); start++) if (it.a == intervals[start].a && it.b == intervals[start].b) break; 

            max_intervals = std::max(max_intervals, greedy_interval_sch_start(start, n, m, intervals));
        }
        max_intervals = std::max(max_intervals, greedy_interval_no_start(current_segment, n, m, intervals));

        std::cout << max_intervals << "\n";
    }
    
    return 0;
}
