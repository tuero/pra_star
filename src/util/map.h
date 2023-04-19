// File: map.h
// Grid based map

#ifndef PRA_UTIL_MAP_H
#define PRA_UTIL_MAP_H

#include <string>
#include <vector>

namespace tpl_search {

// Scenario information
struct Map {
    std::size_t width;
    std::size_t height;
    std::vector<bool> grid_map;
};

/**
 * Load a map
 * @param map_path Path to load from
 * @return The loaded grid map
 */
Map load_map(const std::string &map_path);

}    // namespace tpl_search

#endif    // PRA_UTIL_MAP_H
