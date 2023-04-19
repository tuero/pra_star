// File: a_star.h
// A* search algorithm

#ifndef PRA_ALGORITHM_A_STAR_H
#define PRA_ALGORITHM_A_STAR_H

#include "algorithm/common/graph.h"
#include "algorithm/common/search_output.h"

namespace tpl_search {

/**
 * Perform A* search
 * @param graph The graph to search over
 * @param start_pos The starting position
 * @param goal_pos The goal position
 * @return Results of search
 */
SearchOutput a_star(const FlatGraph &graph, const GridPosition &start_pos, const GridPosition &goal_pos);

}    // namespace tpl_search

#endif    // PRA_ALGORITHM_A_STAR_H
