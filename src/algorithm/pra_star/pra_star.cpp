// File: pra_star.cpp
// PRA* search algorithm

#include "pra_star.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <limits>

#include "algorithm/a_star/a_star.h"
#include "algorithm/common/graph.h"
#include "algorithm/common/graph_generator.h"

namespace tpl_search {

SearchOutput pra_star(HierarchicalGraph &hierarchical_graph, std::size_t k, const GridPosition &start_pos,
                      const GridPosition &goal_pos) {
    std::size_t starting_level = hierarchical_graph.num_layers() / 2;
    std::unordered_set<std::size_t> constrained_nodes;
    SearchOutput search_output, astar_output;
    GridPosition current_start_pos, current_goal_pos;

    // K=0 indicates K=infinity
    if (k < 1) {
        k = std::numeric_limits<std::size_t>::max();
    }

    std::vector<GridPosition> solution_path{start_pos};

    // Loop until we complete an interation with current goal matching target goal
    current_start_pos = start_pos;
    do {
        current_goal_pos = goal_pos;
        for (std::size_t i = 0; i <= starting_level; ++i) {
            std::size_t current_level = starting_level - i;
#ifdef DEBUG
            std::cout << "Searching from " << current_start_pos << " to " << current_goal_pos << " at level "
                      << current_level << std::endl;
#endif

            // Search A* over current graph layer
            FlatGraph &current_graph = hierarchical_graph.get_layer(current_level);
            current_graph.set_constrained_nodes(constrained_nodes);
            astar_output = a_star(current_graph, current_start_pos, current_goal_pos);

            // Truncate to K parameter
            astar_output.path_node_ids.resize(std::min(astar_output.path_node_ids.size(), k));
            auto grid_posisions = current_graph.get_node(astar_output.path_node_ids.back())->represented_positions;
            assert(astar_output.path_node_ids.size() > 0);
            if (i < starting_level) {
                // Find closest abstract node to goal on tail of truncated path
                const auto child_nodes =
                    hierarchical_graph.get_parent_child_mapping(current_level - 1, astar_output.path_node_ids.back());
                FlatGraph &child_graph = hierarchical_graph.get_layer(current_level - 1);
                std::size_t closest_child_id = *std::min_element(
                    child_nodes.begin(), child_nodes.end(), [&](const std::size_t &lhs, const std::size_t &rhs) {
                        auto child_1 = child_graph.get_node(lhs);
                        auto child_2 = child_graph.get_node(rhs);
                        auto lt_grid_pos = [&](const GridPosition &p1, const GridPosition &p2) {
                            return distance(p1, current_goal_pos) < distance(p2, current_goal_pos);
                        };
                        GridPosition p_1 = *std::min_element(child_1->represented_positions.begin(),
                                                             child_1->represented_positions.end(), lt_grid_pos);
                        GridPosition p_2 = *std::min_element(child_2->represented_positions.begin(),
                                                             child_2->represented_positions.end(), lt_grid_pos);

                        return distance(p_1, goal_pos) < distance(p_2, goal_pos);
                    });
                grid_posisions = child_graph.get_node(closest_child_id)->represented_positions;
            }
            // Use closest grid position in this abstract node to represent new goal_pos
            current_goal_pos = *std::min_element(grid_posisions.begin(), grid_posisions.end(),
                                                 [&](const GridPosition &lhs, const GridPosition &rhs) {
                                                     return distance(lhs, goal_pos) < distance(rhs, goal_pos);
                                                 });

            // Set constrained nodes for next level
            constrained_nodes.clear();
            if (i < starting_level) {
                for (const auto &path_node_id : astar_output.path_node_ids) {
                    const auto &child_node_ids =
                        hierarchical_graph.get_parent_child_mapping(current_level - 1, path_node_id);
                    constrained_nodes.insert(child_node_ids.begin(), child_node_ids.end());
                }
            }
            search_output.expanded += astar_output.expanded;
            search_output.generated += astar_output.generated;
            search_output.duration += astar_output.duration;
        }

        // Save the current truncated path at the grounded level
        // Ensure we don't double count start/ends from previous iterations
        for (std::size_t i = 1; i < astar_output.path_node_ids.size(); ++i) {
            solution_path.push_back(*std::begin(
                hierarchical_graph.get_layer(0).get_node(astar_output.path_node_ids[i])->represented_positions));
        }

        // First completion of outer PRA* is time to come up with first move while intermixing planning + acting
        if (search_output.first_move_duration == 0) {
            search_output.first_move_duration = search_output.duration;
        }

        // If truncation occurs, our new start is end of previous path
        current_start_pos = current_goal_pos;
    } while (current_goal_pos != goal_pos);

    // Find total path cost
    double path_cost = 0;
    for (std::size_t i = 1; i < solution_path.size(); ++i) {
        path_cost += distance(solution_path[i], solution_path[i - 1]);
    }
    search_output.path_cost = path_cost;

    return search_output;
}

}    // namespace tpl_search
