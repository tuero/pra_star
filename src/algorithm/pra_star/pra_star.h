// File: pra_star.h
// PRA* search algorithm

#ifndef PRA_ALGORITHM_PRA_STAR_H
#define PRA_ALGORITHM_PRA_STAR_H

#include "algorithm/common/graph.h"
#include "algorithm/common/search_output.h"
#include "util/map.h"

namespace tpl_search {

/**
 * Perform PRA* search
 * @param graph The graph to search over
 * @param k The K parameter for truncation for PRA*
 * @param start_pos The starting position
 * @param goal_pos The goal position
 * @return Results of search
 */
SearchOutput pra_star(HierarchicalGraph &graph, std::size_t k, const GridPosition &start_pos,
                      const GridPosition &goal_pos);

}    // namespace tpl_search

#endif    // PRA_ALGORITHM_PRA_STAR_H
