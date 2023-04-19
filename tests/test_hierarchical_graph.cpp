// File: test_cliques.cpp
// Test the clique abstraction process

#include <filesystem>
#include <iostream>

#include "algorithm/common/graph_generator.h"
#include "test_macros.h"

using namespace tpl_search;

enum Direction {
    UP = 0,
    DOWN = 1,
    LEFT = 2,
    RIGHT = 3,
};

int main() {
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
        HierarchicalGraph hierarchical_graph(graph);

        REQUIRE_TRUE(hierarchical_graph.num_layers() == 3);
        auto node_id = hierarchical_graph.get_layer(2).get_all_node_ids()[0];
        auto positions = hierarchical_graph.get_layer(2).get_node(node_id)->represented_positions;

        // Ensure the last top node covers all grid positions
        for (std::size_t y = 0; y < 4; ++y) {
            for (std::size_t x = 0; x < 4; ++x) {
                REQUIRE_TRUE(positions.find({x, y}) != positions.end());
            }
        }
    }
    // Test saving and loading
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
        std::cout << "test" << std::endl;
        HierarchicalGraph hierarchical_graph_original(graph);
        std::filesystem::path scenario_path(__FILE__);
        scenario_path = scenario_path.replace_filename("hierarchical_graph.nop");
        hierarchical_graph_original.save(scenario_path);

        HierarchicalGraph hierarchical_graph;
        hierarchical_graph.load(scenario_path);

        REQUIRE_TRUE(hierarchical_graph.num_layers() == 3);
        auto node_id = hierarchical_graph.get_layer(2).get_all_node_ids()[0];
        auto positions = hierarchical_graph.get_layer(2).get_node(node_id)->represented_positions;
        // Ensure the last top node covers all grid positions
        for (std::size_t y = 0; y < 4; ++y) {
            for (std::size_t x = 0; x < 4; ++x) {
                REQUIRE_TRUE(positions.find({x, y}) != positions.end());
            }
        }
    }
}
