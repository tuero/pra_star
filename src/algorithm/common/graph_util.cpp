// File: graph_util.pp
// Utility graph generator functions

#include "graph_util.h"

#include <algorithm>

namespace tpl_search {

bool is_candidate(std::size_t node_id, const std::unordered_set<std::size_t> &valid_node_ids,
                  const std::unordered_set<std::size_t> &removed_node_ids, const FlatGraph &graph, std::size_t degree) {
    return valid_node_ids.find(node_id) != valid_node_ids.end() &&
           removed_node_ids.find(node_id) == removed_node_ids.end() && graph.get_node_degree(node_id) >= degree;
}

bool is_clique(const std::vector<std::size_t> &node_ids, const FlatGraph &graph) {
    for (std::size_t i = 0; i < node_ids.size(); ++i) {
        for (std::size_t j = 1; j < node_ids.size(); ++j) {
            if (i != j && !graph.are_neighbours(node_ids[i], node_ids[j])) {
                return false;
            }
        }
    }
    return true;
}

std::vector<Clique> find_cliques_4(std::unordered_set<std::size_t> &node_ids, const FlatGraph &graph) {
    const std::size_t DEGREE = 3;
    std::vector<Clique> cliques;
    std::unordered_set<std::size_t> removed_node_ids;
    std::vector<std::size_t> neighbour_ids1;
    std::vector<std::size_t> neighbour_ids2;
    std::vector<std::size_t> neighbour_ids3;

    auto find_clique = [&](std::size_t node_id) {
        // Node is already used in a clique or doesn't have sufficient neighbours
        if (!is_candidate(node_id, node_ids, removed_node_ids, graph, DEGREE)) {
            return;
        }
        // Node is of sufficient degree, need to check neighbours
        graph.get_neighbours(node_id, neighbour_ids1);
        for (const auto n1_id : neighbour_ids1) {
            if (!is_candidate(n1_id, node_ids, removed_node_ids, graph, DEGREE)) {
                continue;
            }

            // First neighbour is of sufficient degree, need to check its neighbours (excluding first)
            graph.get_neighbours(n1_id, neighbour_ids2);
            for (const auto n2_id : neighbour_ids2) {
                if (n2_id == node_id || !is_candidate(n2_id, node_ids, removed_node_ids, graph, DEGREE)) {
                    continue;
                }
                // Second neighbour is not repeated first, and is of sufficient degree
                graph.get_neighbours(n2_id, neighbour_ids3);
                for (const auto n3_id : neighbour_ids3) {
                    if (n3_id == node_id || n3_id == n1_id ||
                        !is_candidate(n3_id, node_ids, removed_node_ids, graph, DEGREE)) {
                        continue;
                    }

                    // All nodes are candidate nodes (degree 3 and no copies)
                    // Need to ensure it forms a clique
                    if (is_clique({node_id, n1_id, n2_id, n3_id}, graph)) {
                        cliques.push_back({node_id, n1_id, n2_id, n3_id});
                        removed_node_ids.insert(node_id);
                        removed_node_ids.insert(n1_id);
                        removed_node_ids.insert(n2_id);
                        removed_node_ids.insert(n3_id);
                        return;
                    }
                }
            }
        }
    };

    for (const auto &node_id : node_ids) {
        find_clique(node_id);
    }

    // remove nodes chosen as part of cliques
    for (const auto &node_id : removed_node_ids) {
        node_ids.erase(node_id);
    }
    return cliques;
}

std::vector<Clique> find_cliques_3(std::unordered_set<std::size_t> &node_ids, const FlatGraph &graph) {
    const std::size_t DEGREE = 2;
    std::vector<Clique> cliques;
    std::unordered_set<std::size_t> removed_node_ids;
    std::vector<std::size_t> neighbour_ids1;
    std::vector<std::size_t> neighbour_ids2;

    auto find_clique = [&](std::size_t node_id) {
        // Node is already used in a clique or doesn't have sufficient neighbours
        if (!is_candidate(node_id, node_ids, removed_node_ids, graph, DEGREE)) {
            return;
        }
        // Node is of sufficient degree, need to check neighbours
        graph.get_neighbours(node_id, neighbour_ids1);
        for (const auto n1_id : neighbour_ids1) {
            if (!is_candidate(n1_id, node_ids, removed_node_ids, graph, DEGREE)) {
                continue;
            }

            // First neighbour is of sufficient degree, need to check its neighbours (excluding first)
            graph.get_neighbours(n1_id, neighbour_ids2);
            for (const auto n2_id : neighbour_ids2) {
                if (n2_id == node_id || !is_candidate(n2_id, node_ids, removed_node_ids, graph, DEGREE)) {
                    continue;
                }
                // Second neighbour is not repeated first, and is of sufficient degree

                // All nodes are candidate nodes (degree 3 and no copies)
                // Need to ensure it forms a clique
                if (is_clique({node_id, n1_id, n2_id}, graph)) {
                    cliques.push_back({node_id, n1_id, n2_id});
                    removed_node_ids.insert(node_id);
                    removed_node_ids.insert(n1_id);
                    removed_node_ids.insert(n2_id);
                    return;
                }
            }
        }
    };

    for (const auto &node_id : node_ids) {
        find_clique(node_id);
    }

    // remove nodes chosen as part of cliques
    for (const auto &node_id : removed_node_ids) {
        node_ids.erase(node_id);
    }
    return cliques;
}

std::vector<Clique> find_cliques_2(std::unordered_set<std::size_t> &node_ids, const FlatGraph &graph) {
    const std::size_t DEGREE = 1;
    std::vector<Clique> cliques;
    std::unordered_set<std::size_t> removed_node_ids;
    std::vector<std::size_t> neighbour_ids1;

    auto find_clique = [&](std::size_t node_id) {
        // Node is already used in a clique or doesn't have sufficient neighbours
        if (!is_candidate(node_id, node_ids, removed_node_ids, graph, DEGREE)) {
            return;
        }
        // Node is of sufficient degree, need to check neighbours
        graph.get_neighbours(node_id, neighbour_ids1);
        for (const auto n1_id : neighbour_ids1) {
            if (!is_candidate(n1_id, node_ids, removed_node_ids, graph, DEGREE)) {
                continue;
            }

            cliques.push_back({node_id, n1_id});
            removed_node_ids.insert(node_id);
            removed_node_ids.insert(n1_id);
            return;
        }
    };

    for (const auto &node_id : node_ids) {
        find_clique(node_id);
    }

    // remove nodes chosen as part of cliques
    for (const auto &node_id : removed_node_ids) {
        node_ids.erase(node_id);
    }
    return cliques;
}

AbstractPosition average_position(const Clique &clique, const FlatGraph &graph) {
    int N = 0;
    double x = 0;
    double y = 0;

    for (const auto node_id : clique) {
        for (const auto &position : graph.get_node(node_id)->represented_positions) {
            x += static_cast<double>(position.x);
            y += static_cast<double>(position.y);
            ++N;
        }
    }

    return {x / N, y / N};
}

std::unordered_set<GridPosition, PairHash> collect_grid_positions(const Clique &clique, const FlatGraph &graph) {
    std::unordered_set<GridPosition, PairHash> represented_positions;

    for (const auto node_id : clique) {
        const std::unordered_set<GridPosition, PairHash> &clique_positons =
            graph.get_node(node_id)->represented_positions;
        represented_positions.insert(clique_positons.begin(), clique_positons.end());
    }

    return represented_positions;
}

FlatGraph create_abstract_graph(const FlatGraph &graph, HierarchicalGraph::ParentChildMap &parent_child_mapping) {
    std::vector<std::size_t> all_node_ids = graph.get_all_node_ids();
    std::unordered_set<std::size_t> current_node_ids(all_node_ids.begin(), all_node_ids.end());

    // Get cliques
    std::vector<Clique> cliques_4 = find_cliques_4(current_node_ids, graph);
    std::vector<Clique> cliques_3 = find_cliques_3(current_node_ids, graph);
    std::vector<Clique> cliques_2 = find_cliques_2(current_node_ids, graph);
    std::vector<Clique> cliques_all;
    cliques_all.insert(cliques_all.end(), cliques_4.begin(), cliques_4.end());
    cliques_all.insert(cliques_all.end(), cliques_3.begin(), cliques_3.end());
    cliques_all.insert(cliques_all.end(), cliques_2.begin(), cliques_2.end());

    // Check for islands
    std::size_t id_counter = 0;
    std::unordered_map<std::size_t, std::size_t> node_id_to_clique;
    // Map how nodes relate to cliques
    for (const auto &clique : cliques_all) {
        for (const auto &node_id : clique) {
            node_id_to_clique[node_id] = id_counter;
        }
        ++id_counter;
    }
    std::unordered_set<std::size_t> island_node_ids;
    int island_counter = 0;
    // Find islands and add to clique its joined to
    for (const auto &node_id : current_node_ids) {
        const auto neighbours = graph.get_neighbours(node_id);
        if (neighbours.size() == 1) {
            cliques_all[node_id_to_clique.at(neighbours.at(0))].push_back(node_id);
            island_node_ids.insert(node_id);
            ++island_counter;
        }
    }
    // Erase nodes in islands from rest used for cliques of size 1
    for (const auto &node_id : island_node_ids) {
        current_node_ids.erase(node_id);
    }

    std::cout << "Clique found, 4: " << cliques_4.size() << ", 3: " << cliques_3.size() << ", 2: " << cliques_2.size()
              << ", 1: " << current_node_ids.size() << ", Islands: " << island_counter << std::endl;

    FlatGraph abstract_graph;
    id_counter = 0;

    // Create nodes for each clique
    for (const auto &clique : cliques_all) {
        abstract_graph.add_node({id_counter, average_position(clique, graph), collect_grid_positions(clique, graph)});
        parent_child_mapping[id_counter].insert(clique.begin(), clique.end());
        for (const auto &node_id : clique) {
            node_id_to_clique[node_id] = id_counter;
        }
        ++id_counter;
    }
    // Leftover nodes not islands are of clique size 1
    for (const auto &clique_single : current_node_ids) {
        abstract_graph.add_node({id_counter, average_position(Clique{clique_single}, graph),
                                 collect_grid_positions(Clique{clique_single}, graph)});
        parent_child_mapping[id_counter].insert(clique_single);
        ++id_counter;
    }

    auto are_neighbours = [&](const std::unordered_set<std::size_t> &child_ids1,
                              const std::unordered_set<std::size_t> &child_ids2) -> bool {
        for (const auto &child_id1 : child_ids1) {
            for (const auto &child_id2 : child_ids2) {
                if (graph.are_neighbours(child_id1, child_id2)) {
                    return true;
                }
            }
        }
        return false;
    };

    // Add edges for each clique neighbour
    for (std::size_t i = 0; i < parent_child_mapping.size(); ++i) {
        for (std::size_t j = i + 1; j < parent_child_mapping.size(); ++j) {
            if (are_neighbours(parent_child_mapping.at(i), parent_child_mapping.at(j))) {
                abstract_graph.add_edge(i, j);
            }
        }
    }
    return abstract_graph;
}

}    // namespace tpl_search
