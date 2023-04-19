// File: search_output.h
// Result of search output

#ifndef PRA_ALGORITHM_COMMON_SEARCH_OUTPUT_H
#define PRA_ALGORITHM_COMMON_SEARCH_OUTPUT_H

#include <vector>

namespace tpl_search {

// Result of search algorithm
struct SearchOutput {
    std::size_t expanded = 0;
    std::size_t generated = 0;
    double duration = 0;
    double first_move_duration = 0;
    double path_cost = 0;
    std::vector<std::size_t> path_node_ids;
};

}    // namespace tpl_search

#endif    // PRA_ALGORITHM_COMMON_SEARCH_OUTPUT_H
