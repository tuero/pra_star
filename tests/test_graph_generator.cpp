// File: test_graph_generator.cpp
// Test the graph generator from a scenario

#include <filesystem>
#include <iostream>

#include "algorithm/common/graph_generator.h"
#include "test_macros.h"
#include "util/file_util.h"
#include "util/map.h"
#include "util/scenario.h"

using namespace tpl_search;

int main() {
    std::filesystem::path scenario_path(__FILE__);
    scenario_path = scenario_path.replace_filename("battleground.map.scen");
    std::filesystem::path map_path = scenario_to_map_path(scenario_path);
    Scenario scenario = load_scenario(scenario_path, 0);
    Map map = load_map(map_path);
    {
        REQUIRE_TRUE(map.grid_map[scenario.start_y * scenario.width + scenario.start_x] == 1);
        FlatGraph graph = load_flat_graph(scenario_to_map_path(scenario_path));
        const GraphNode *start_node = graph.get_node(scenario.start_y * scenario.width + scenario.start_x);
        std::vector<std::size_t> neighbours;
        graph.get_neighbours(start_node->id, neighbours);
        REQUIRE_TRUE(neighbours.size() == 8);
        std::unordered_set<std::size_t> neighbour_ids = {
            (scenario.width * (scenario.start_y - 1) + (scenario.start_x)),
            (scenario.width * (scenario.start_y + 1) + (scenario.start_x)),
            (scenario.width * (scenario.start_y) + (scenario.start_x - 1)),
            (scenario.width * (scenario.start_y) + (scenario.start_x + 1)),
            (scenario.width * (scenario.start_y - 1) + (scenario.start_x - 1)),
            (scenario.width * (scenario.start_y + 1) + (scenario.start_x - 1)),
            (scenario.width * (scenario.start_y - 1) + (scenario.start_x + 1)),
            (scenario.width * (scenario.start_y + 1) + (scenario.start_x + 1)),
        };
        for (auto const &neighbour_id : neighbours) {
            REQUIRE_TRUE(neighbour_ids.find(neighbour_id) != neighbour_ids.end());
            neighbour_ids.erase(neighbour_id);
        }
    }
    {
        // Test saving/loading
        FlatGraph graph_original = load_flat_graph(map_path);
        graph_original.save(map_to_flat_graph_path(map_path));
        FlatGraph graph;
        graph.load(map_to_flat_graph_path(map_path));
        const GraphNode *start_node = graph.get_node(scenario.start_y * scenario.width + scenario.start_x);
        std::vector<std::size_t> neighbours;
        graph.get_neighbours(start_node->id, neighbours);
        REQUIRE_TRUE(neighbours.size() == 8);
        std::unordered_set<std::size_t> neighbour_ids = {
            (scenario.width * (scenario.start_y - 1) + (scenario.start_x)),
            (scenario.width * (scenario.start_y + 1) + (scenario.start_x)),
            (scenario.width * (scenario.start_y) + (scenario.start_x - 1)),
            (scenario.width * (scenario.start_y) + (scenario.start_x + 1)),
            (scenario.width * (scenario.start_y - 1) + (scenario.start_x - 1)),
            (scenario.width * (scenario.start_y + 1) + (scenario.start_x - 1)),
            (scenario.width * (scenario.start_y - 1) + (scenario.start_x + 1)),
            (scenario.width * (scenario.start_y + 1) + (scenario.start_x + 1)),
        };
        for (auto const &neighbour_id : neighbours) {
            REQUIRE_TRUE(neighbour_ids.find(neighbour_id) != neighbour_ids.end());
            neighbour_ids.erase(neighbour_id);
        }
    }
    {
        std::size_t start_x = 119;
        REQUIRE_TRUE(map.grid_map[scenario.start_y * scenario.width + start_x] == 1);
        FlatGraph graph = load_flat_graph(map_path);
        const GraphNode *start_node = graph.get_node(scenario.start_y * scenario.width + start_x);
        std::vector<std::size_t> neighbours;
        graph.get_neighbours(start_node->id, neighbours);
        REQUIRE_TRUE(neighbours.size() == 5);
        std::unordered_set<std::size_t> neighbour_ids = {
            (scenario.width * (scenario.start_y - 1) + (start_x)),
            (scenario.width * (scenario.start_y + 1) + (start_x)),
            (scenario.width * (scenario.start_y) + (start_x + 1)),
            (scenario.width * (scenario.start_y - 1) + (start_x + 1)),
            (scenario.width * (scenario.start_y + 1) + (start_x + 1)),
        };
        for (auto const &neighbour_id : neighbours) {
            REQUIRE_TRUE(neighbour_ids.find(neighbour_id) != neighbour_ids.end());
            neighbour_ids.erase(neighbour_id);
        }
    }
}
