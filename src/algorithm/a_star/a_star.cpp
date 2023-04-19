// File: a_star.cpp
// A* search algorithm

#include "a_star.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <memory>

#include "util/priority_queue.h"
#include "util/timer.h"

namespace tpl_search {

constexpr double EPS = 1e-5;

bool is_greater(double lhs, double rhs) {
    return lhs > rhs + EPS;
}

// Node used in search
struct SearchNode {
    const SearchNode *parent = nullptr;
    const GraphNode *graph_node;
    double g = 0;
    double f = 0;

    // Compare function for nodes (f-cost, then g-cost on tiebreaks)
    struct CompareOrdered {
        bool operator()(const SearchNode &left, const SearchNode &right) const {
            return left.f < right.f || (left.f == right.f && left.g > right.g);
        }
    };

    // Hashing
    struct Hasher {
        using is_transparent = void;
        std::size_t operator()(const SearchNode &node) const {
            return node.graph_node->id;
        }
        std::size_t operator()(const std::unique_ptr<SearchNode> &node) const {
            return node->graph_node->id;
        }
        std::size_t operator()(std::size_t id) const {
            return id;
        }
    };

    // Node equality compare (underlying state) for closed
    struct CompareEqual {
        using is_transparent = void;
        bool operator()(const SearchNode &left, const SearchNode &right) const {
            return *left.graph_node == *right.graph_node;
        }
        bool operator()(const std::unique_ptr<SearchNode> &left, const std::unique_ptr<SearchNode> &right) const {
            return *left->graph_node == *right->graph_node;
        }
        bool operator()(const SearchNode &left, std::size_t id) const {
            return left.graph_node->id == id;
        }
        bool operator()(const std::unique_ptr<SearchNode> &left, std::size_t id) const {
            return left->graph_node->id == id;
        }
        bool operator()(std::size_t id, const SearchNode &right) const {
            return id == right.graph_node->id;
        }
        bool operator()(std::size_t id, const std::unique_ptr<SearchNode> &right) const {
            return id == right->graph_node->id;
        }
    };
};

std::vector<std::size_t> reconstruct_path(const SearchNode *current_node) {
    assert(current_node);
    std::vector<std::size_t> path;
    while (current_node) {
        path.push_back(current_node->graph_node->id);
        current_node = current_node->parent;
    }
    std::reverse(path.begin(), path.end());
    return path;
}

SearchOutput a_star(const FlatGraph &graph, const GridPosition &start_pos, const GridPosition &goal_pos) {
    PrioritySet<SearchNode, SearchNode::CompareOrdered, SearchNode::Hasher> open;
    std::unordered_set<std::unique_ptr<SearchNode>, SearchNode::Hasher, SearchNode::CompareEqual> closed;
    std::vector<std::size_t> neighbour_ids;

    // Init
    std::size_t expanded = 0;
    std::size_t generated = 0;

    ThreadTimer timer;
    timer.start();

    const GraphNode *start_node = graph.get_node(graph.get_pos_node_id(start_pos));
    const GraphNode *goal_node = graph.get_node(graph.get_pos_node_id(goal_pos));
    open.insert({nullptr, start_node, 0, 0 + distance(start_node, goal_node)});
    while (!open.empty()) {
        SearchNode current = open.top();
        closed.insert(std::make_unique<SearchNode>(open.top()));
        open.pop();
        const SearchNode *current_ptr = (*closed.find(current.graph_node->id)).get();
        ++expanded;

        // Goal check
        if (current.graph_node == goal_node) {
            double duration = timer.get_duration();
            const SearchOutput search_output{expanded, generated, duration,
                                             duration, current.g, reconstruct_path(current_ptr)};
#ifdef DEBUG
            std::cout << "Solution found. Solution length: " << search_output.path_node_ids.size()
                      << ", solution cost: " << current.g << ", Expanded: " << expanded << ", Generated: " << generated
                      << ", Time: " << duration << "s" << std::endl;
#endif
            return search_output;
        }

        auto consider_child = [&](SearchNode &child_node) -> bool {
            // Check closed for re-expansion
            auto closed_iter = closed.find(child_node.graph_node->id);
            if (closed_iter != closed.end()) {
                // Technically not needed for consistent heuristic
                if (is_greater((*closed_iter)->g, child_node.g + EPS)) {
                    closed.erase(closed_iter);
                    open.insert(std::move(child_node));
                    return true;
                }
            }
            // Check open for better child found
            else if (open.contains(child_node)) {
                double g = open.get(child_node.graph_node->id).g;
                if (is_greater(g, child_node.g)) {
                    open.update(child_node);
                    return true;
                }
            } else {
                open.insert(std::move(child_node));
                return true;
            }
            return false;
        };

        // Generate children
        graph.get_neighbours(current.graph_node->id, neighbour_ids);
        for (auto const &neighbour_id : neighbour_ids) {
            const GraphNode *neighbour_node = graph.get_node(neighbour_id);
            double delta_g = distance(current.graph_node, neighbour_node);
            double child_g = current.g + delta_g;
            double child_h = distance(neighbour_node, goal_node);
            assert(!is_greater(current.f - current.g, delta_g + child_h));
            SearchNode child_node{current_ptr, neighbour_node, child_g, child_g + child_h};
            generated += consider_child(child_node);
        }
    }
#ifdef DEBUG
    std::cerr << "Exhausted search space, no solution found" << std::endl;
#endif
    return {expanded, generated, timer.get_duration(), -1, -1, {}};
}

}    // namespace tpl_search
