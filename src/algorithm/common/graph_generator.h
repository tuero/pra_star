// File: graph_generator.h
// Utility graph generator functions

#ifndef PRA_ALGORITHM_COMMON_GRAPH_GENERATOR_H
#define PRA_ALGORITHM_COMMON_GRAPH_GENERATOR_H

#include <array>

#include "graph.h"
#include "util/scenario.h"

namespace tpl_search {

/**
 * Load flat graph from map path
 * @note If result hasn't been cached to disk, will be created on the fly
 * @param map_path Path to map file
 * @param force_create Force create the graph even if it already exists on disk
 * @return Flat graph representing map
 */
FlatGraph load_flat_graph(const std::string &map_path, bool force_create = false);

/**
 * Load hierarchical graph from map path
 * @note If result hasn't been cached to disk, will be created on the fly
 * @param map_path Path to map file
 * @param force_create Force create the graph even if it already exists on disk
 * @return Hierarchical graph representing map
 */
HierarchicalGraph load_hierarchical_graph(const std::string &map_path, bool force_create = false);

}    // namespace tpl_search

#endif    // PRA_ALGORITHM_COMMON_GRAPH_GENERATOR_H
