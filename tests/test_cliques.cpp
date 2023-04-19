// File: test_cliques.cpp
// Test the clique abstraction process

#include <filesystem>
#include <iostream>

#include "algorithm/common/graph_generator.h"
#include "algorithm/common/graph_util.h"
#include "test_macros.h"

using namespace tpl_search;

enum Direction {
    UP = 0,
    DOWN = 1,
    LEFT = 2,
    RIGHT = 3,
};

int main() {
    // Test with no edges
    {
        FlatGraph graph;
        for (std::size_t y = 0; y < 4; ++y) {
            for (std::size_t x = 0; x < 4; ++x) {
                graph.add_node({y * 4 + x, {static_cast<double>(x), static_cast<double>(y)}, {{y, x}}});
            }
        }
        std::vector<std::size_t> node_ids_all = graph.get_all_node_ids();
        std::unordered_set<std::size_t> node_ids(node_ids_all.begin(), node_ids_all.end());
        std::vector<Clique> cliques = find_cliques_4(node_ids, graph);
        REQUIRE_TRUE(cliques.size() == 0);
        REQUIRE_TRUE(node_ids.size() == 4 * 4);
    }
    // Test with all grid edges
    {
        FlatGraph graph;
        for (std::size_t y = 0; y < 4; ++y) {
            for (std::size_t x = 0; x < 4; ++x) {
                graph.add_node({y * 4 + x, {static_cast<double>(x), static_cast<double>(y)}, {{y, x}}});
            }
        }
        for (std::size_t y = 0; y < 4; ++y) {
            for (std::size_t x = 0; x < 4; ++x) {
                std::size_t id = y * 4 + x;
                std::array<bool, 4> flags{};

                // Check cardinal directions
                if (y > 0) {
                    graph.add_edge(id, id - 4);
                    flags[Direction::UP] = true;
                }
                if (y < 4 - 1) {
                    graph.add_edge(id, id + 4);
                    flags[Direction::DOWN] = true;
                }
                if (x > 0) {
                    graph.add_edge(id, id - 1);
                    flags[Direction::LEFT] = true;
                }
                if (x < 4 - 1) {
                    graph.add_edge(id, id + 1);
                    flags[Direction::RIGHT] = true;
                }

                // Check diagonal directions
                // This requires both ways around to avoid clipping
                if (flags[Direction::UP] && flags[Direction::LEFT]) {
                    graph.add_edge(id, id - 4 - 1);
                }
                if (flags[Direction::UP] && flags[Direction::RIGHT]) {
                    graph.add_edge(id, id - 4 + 1);
                }
                if (flags[Direction::DOWN] && flags[Direction::LEFT]) {
                    graph.add_edge(id, id + 4 - 1);
                }
                if (flags[Direction::DOWN] && flags[Direction::RIGHT]) {
                    graph.add_edge(id, id + 4 + 1);
                }
            }
        }
        std::vector<std::size_t> node_ids_all = graph.get_all_node_ids();
        std::unordered_set<std::size_t> node_ids(node_ids_all.begin(), node_ids_all.end());
        std::vector<Clique> cliques = find_cliques_4(node_ids, graph);
        for (const auto& clique : cliques) {
            for (const auto& id : clique) {
                std::cout << id << " ";
            }
            std::cout << std::endl;
        }
        REQUIRE_TRUE(cliques.size() == 4);
    }
    // Test with all grid edges but for cliques of 3
    {
        FlatGraph graph;
        for (std::size_t y = 0; y < 4; ++y) {
            for (std::size_t x = 0; x < 4; ++x) {
                graph.add_node({y * 4 + x, {static_cast<double>(x), static_cast<double>(y)}, {{y, x}}});
            }
        }
        for (std::size_t y = 0; y < 4; ++y) {
            for (std::size_t x = 0; x < 4; ++x) {
                std::size_t id = y * 4 + x;
                std::array<bool, 4> flags{};

                // Check cardinal directions
                if (y > 0) {
                    graph.add_edge(id, id - 4);
                    flags[Direction::UP] = true;
                }
                if (y < 4 - 1) {
                    graph.add_edge(id, id + 4);
                    flags[Direction::DOWN] = true;
                }
                if (x > 0) {
                    graph.add_edge(id, id - 1);
                    flags[Direction::LEFT] = true;
                }
                if (x < 4 - 1) {
                    graph.add_edge(id, id + 1);
                    flags[Direction::RIGHT] = true;
                }

                // Check diagonal directions
                // This requires both ways around to avoid clipping
                if (flags[Direction::UP] && flags[Direction::LEFT]) {
                    graph.add_edge(id, id - 4 - 1);
                }
                if (flags[Direction::DOWN] && flags[Direction::RIGHT]) {
                    graph.add_edge(id, id + 4 + 1);
                }
            }
        }
        std::vector<std::size_t> node_ids_all = graph.get_all_node_ids();
        std::unordered_set<std::size_t> node_ids(node_ids_all.begin(), node_ids_all.end());
        std::vector<Clique> cliques = find_cliques_3(node_ids, graph);
        for (const auto& clique : cliques) {
            for (const auto& id : clique) {
                std::cout << id << " ";
            }
            std::cout << std::endl;
        }
        REQUIRE_TRUE(cliques.size() == 4);
    }
    // Test with all grid edges for clique size of 2
    {
        FlatGraph graph;
        for (std::size_t y = 0; y < 4; ++y) {
            for (std::size_t x = 0; x < 4; ++x) {
                graph.add_node({y * 4 + x, {static_cast<double>(x), static_cast<double>(y)}, {{y, x}}});
            }
        }
        for (std::size_t y = 0; y < 4; ++y) {
            for (std::size_t x = 0; x < 4; ++x) {
                std::size_t id = y * 4 + x;

                // Check cardinal directions
                if (y > 0) {
                    graph.add_edge(id, id - 4);
                }
                if (y < 4 - 1) {
                    graph.add_edge(id, id + 4);
                }
                if (x > 0) {
                    graph.add_edge(id, id - 1);
                }
                if (x < 4 - 1) {
                    graph.add_edge(id, id + 1);
                }
            }
        }
        std::vector<std::size_t> node_ids_all = graph.get_all_node_ids();
        std::unordered_set<std::size_t> node_ids(node_ids_all.begin(), node_ids_all.end());
        std::vector<Clique> cliques = find_cliques_2(node_ids, graph);
        for (const auto& clique : cliques) {
            for (const auto& id : clique) {
                std::cout << id << " ";
            }
            std::cout << std::endl;
        }
        REQUIRE_TRUE(cliques.size() == 8);
        REQUIRE_TRUE(node_ids.size() == 0);
    }
    // Test abstraction
    {
        FlatGraph graph;
        for (std::size_t y = 0; y < 4; ++y) {
            for (std::size_t x = 0; x < 4; ++x) {
                graph.add_node({y * 4 + x, {static_cast<double>(x), static_cast<double>(y)}, {{y, x}}});
            }
        }
        for (std::size_t y = 0; y < 4; ++y) {
            for (std::size_t x = 0; x < 4; ++x) {
                std::size_t id = y * 4 + x;
                std::array<bool, 4> flags{};

                // Check cardinal directions
                if (y > 0) {
                    graph.add_edge(id, id - 4);
                    flags[Direction::UP] = true;
                }
                if (y < 4 - 1) {
                    graph.add_edge(id, id + 4);
                    flags[Direction::DOWN] = true;
                }
                if (x > 0) {
                    graph.add_edge(id, id - 1);
                    flags[Direction::LEFT] = true;
                }
                if (x < 4 - 1) {
                    graph.add_edge(id, id + 1);
                    flags[Direction::RIGHT] = true;
                }

                // Check diagonal directions
                // This requires both ways around to avoid clipping
                if (flags[Direction::UP] && flags[Direction::LEFT]) {
                    graph.add_edge(id, id - 4 - 1);
                }
                if (flags[Direction::UP] && flags[Direction::RIGHT]) {
                    graph.add_edge(id, id - 4 + 1);
                }
                if (flags[Direction::DOWN] && flags[Direction::LEFT]) {
                    graph.add_edge(id, id + 4 - 1);
                }
                if (flags[Direction::DOWN] && flags[Direction::RIGHT]) {
                    graph.add_edge(id, id + 4 + 1);
                }
            }
        }
        HierarchicalGraph::ParentChildMap parent_child_map;
        FlatGraph abstract_graph = create_abstract_graph(graph, parent_child_map);
        std::vector<std::size_t> node_ids = abstract_graph.get_all_node_ids();
        REQUIRE_TRUE(node_ids.size() == 4);
        for (const auto& node_id : node_ids) {
            for (const auto& pos : abstract_graph.get_node(node_id)->represented_positions) {
                std::cout << "(" << pos.x << ", " << pos.y << ") ";
            }
            std::cout << std::endl;
        }
    }
}
