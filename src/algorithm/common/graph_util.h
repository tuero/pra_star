// File: graph_util.h
// Utility graph generator functions

#ifndef PRA_ALGORITHM_COMMON_GRAPH_UTIL_H
#define PRA_ALGORITHM_COMMON_GRAPH_UTIL_H

#include <array>

#include "graph.h"

namespace tpl_search {

// Clique typedef
using Clique = std::vector<std::size_t>;

/**
 * Find all cliques of size 4
 * @param node_ids Node IDs from graph which are valid to place in clique
 * @param graph Current graph layer
 * @return Vector of node IDs representing cliques
 */
std::vector<Clique> find_cliques_4(std::unordered_set<std::size_t> &node_ids, const FlatGraph &graph);

/**
 * Find all cliques of size 3
 * @param node_ids Node IDs from graph which are valid to place in clique
 * @param graph Current graph layer
 * @return Vector of node IDs representing cliques
 */
std::vector<Clique> find_cliques_3(std::unordered_set<std::size_t> &node_ids, const FlatGraph &graph);

/**
 * Find all cliques of size 2
 * @param node_ids Node IDs from graph which are valid to place in clique
 * @param graph Current graph layer
 * @return Vector of node IDs representing cliques
 */
std::vector<Clique> find_cliques_2(std::unordered_set<std::size_t> &node_ids, const FlatGraph &graph);

/**
 * Create abstract graph from previous layer
 * @param graph Current graph layer
 * @param parent_child_mapping Reference to place parent child mappings in for next layer
 * @return Flat graph representing the next layer of abstraction
 */
FlatGraph create_abstract_graph(const FlatGraph &graph, HierarchicalGraph::ParentChildMap &parent_child_mapping);

}    // namespace tpl_search

#endif    // PRA_ALGORITHM_COMMON_GRAPH_UTIL_H
