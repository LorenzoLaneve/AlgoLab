#ifndef __interval_scheduling
#define __interval_scheduling

#include <vector>
#include <algorithm>

long interval_scheduling(const std::vector<std::pair<long, long>> &intervals) {
    std::vector<std::pair<long, long>> sorted_ints(intervals.begin(), intervals.end());

    std::sort(sorted_ints.begin(), sorted_ints.end(),
        [&](const std::pair<long, long> &i, const std::pair<long, long> &j) {
            return i.second < j.second;
        });

    long taken_ints = 1;
    long first_free = intervals[0].second + 1;
    for (long i = 1; i < intervals.size(); i++) {
        if (first_free <= intervals[i].first) {
            taken_ints++;
            first_free = intervals[i].second + 1;
        }
    }

    return taken_ints;
}

#endif