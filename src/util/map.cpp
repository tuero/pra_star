// File: map.cpp
// Grid based map

#include "map.h"

#include <cassert>
#include <fstream>
#include <unordered_set>

#include "file_util.h"

namespace tpl_search {

const std::unordered_set<char> PATHABLE_MAP{'.', 'S'};

Map load_map(const std::string &map_path) {
    std::ifstream map_file(map_path);

    // Map data header
    std::string s_type, s_type_val, s_height, s_width, s_map;
    std::size_t i_height_val, i_width_val;
    map_file >> s_type >> s_type_val >> s_height >> i_height_val >> s_width >> i_width_val >> s_map;
    check_input_stream("file header");

    std::vector<char> map_grid_raw;
    char current_char;
    for (std::size_t i = 0; i < i_width_val * i_height_val; ++i) {
        map_file >> current_char;
        check_input_stream("map data");
        map_grid_raw.push_back(current_char);
    }
    assert(!map_grid_raw.empty());

    std::vector<bool> map_grid;
    map_grid.reserve(map_grid_raw.size());
    for (auto const &v : map_grid_raw) {
        map_grid.push_back(PATHABLE_MAP.find(v) != PATHABLE_MAP.end());
    }
    return {i_width_val, i_height_val, map_grid};
}

}    // namespace tpl_search
