// File: graph_generator.pp
// Utility graph generator functions

#include "graph_generator.h"

#include <algorithm>
#include <filesystem>
#include <iostream>

#include "util/file_util.h"
#include "util/map.h"
#include "util/timer.h"

namespace tpl_search {

enum Direction {
    UP = 0,
    DOWN = 1,
    LEFT = 2,
    RIGHT = 3,
};

FlatGraph load_flat_graph(const std::string &map_path, bool force_create) {
    FlatGraph graph;

    // Check if flat graph is already cached
    std::filesystem::path flat_graph_path = map_to_flat_graph_path(map_path);
    if (std::filesystem::exists(flat_graph_path) && !force_create) {
        graph.load(flat_graph_path);
        return graph;
    }

    // Otherwise we need to parse and create
    Map map = load_map(map_path);

    // Create nodes
    for (std::size_t y = 0; y < map.height; ++y) {
        for (std::size_t x = 0; x < map.width; ++x) {
            std::size_t id = y * map.width + x;
            if (!map.grid_map[id]) {
                continue;
            }
            graph.add_node({id, {static_cast<double>(x), static_cast<double>(y)}, {{x, y}}});
        }
    }

    // Join neighbours
    for (std::size_t y = 0; y < map.height; ++y) {
        for (std::size_t x = 0; x < map.width; ++x) {
            std::size_t id = y * map.width + x;
            // skip if this cell is blocked
            if (!map.grid_map[id]) {
                continue;
            }

            std::array<bool, 4> flags{};

            // Check cardinal directions
            if (y > 0 && map.grid_map.at(id - map.width)) {
                graph.add_edge(id, id - map.width);
                flags[Direction::UP] = true;
            }
            if (y < map.height - 1 && map.grid_map.at(id + map.width)) {
                graph.add_edge(id, id + map.width);
                flags[Direction::DOWN] = true;
            }
            if (x > 0 && map.grid_map.at(id - 1)) {
                graph.add_edge(id, id - 1);
                flags[Direction::LEFT] = true;
            }
            if (x < map.width - 1 && map.grid_map.at(id + 1)) {
                graph.add_edge(id, id + 1);
                flags[Direction::RIGHT] = true;
            }

            // Check diagonal directions
            // This requires both ways around to avoid clipping
            if (flags[Direction::UP] && flags[Direction::LEFT] && map.grid_map.at(id - map.width - 1)) {
                graph.add_edge(id, id - map.width - 1);
            }
            if (flags[Direction::UP] && flags[Direction::RIGHT] && map.grid_map.at(id - map.width + 1)) {
                graph.add_edge(id, id - map.width + 1);
            }
            if (flags[Direction::DOWN] && flags[Direction::LEFT] && map.grid_map.at(id + map.width - 1)) {
                graph.add_edge(id, id + map.width - 1);
            }
            if (flags[Direction::DOWN] && flags[Direction::RIGHT] && map.grid_map.at(id + map.width + 1)) {
                graph.add_edge(id, id + map.width + 1);
            }
        }
    }

    return graph;
}

HierarchicalGraph load_hierarchical_graph(const std::string &map_path, bool force_create) {
    // Check if flat graph is already cached
    std::filesystem::path hierarchical_graph_path = map_to_hierarchical_graph_path(map_path);
    if (std::filesystem::exists(hierarchical_graph_path) && !force_create) {
        HierarchicalGraph hierarchical_graph;
        hierarchical_graph.load(hierarchical_graph_path);
        return hierarchical_graph;
    }

    // Otherwise we need to parse and create
    FlatGraph flat_graph = load_flat_graph(map_path, force_create);
    return HierarchicalGraph(flat_graph);
}

}    // namespace tpl_search
