// File: file_util.cpp
// File utility

#include "file_util.h"

#include <fstream>
#include <iostream>

namespace tpl_search {

std::ifstream open_and_validate_file(const std::string &file_path) {
    // File exists
    if (!std::filesystem::exists(file_path)) {
        std::cerr << "Error: Scenario file " << file_path << " does not exist." << std::endl;
        exit(1);
    }
    // File can open properly
    std::ifstream file_stream(file_path.c_str());
    if (!file_stream) {
        std::cerr << "Error: failed to open file " << file_path << std::endl;
        exit(1);
    }
    return file_stream;
}

void check_input_stream(const std::string &message) {
    if (!std::cin) {
        if (std::cin.eof()) {
            std::cerr << "Error: End of file while reading " << message << std::endl;
        } else {
            std::cerr << "Error: Failure while reading " << message << std::endl;
        }
        std::exit(1);
    }
}

std::filesystem::path scenario_to_map_path(const std::filesystem::path &scenario_path) {
    // ./AR00011SR.map.scen to ./AR00011SR.map
    std::filesystem::path map_path = scenario_path;
    return map_path.replace_filename(map_path.stem());
}

std::filesystem::path map_to_flat_graph_path(const std::filesystem::path &map_path) {
    // ./AR00011SR.map to ./AR00011SR.flat_graph.nop
    std::filesystem::path flat_graph_path = map_path;
    return flat_graph_path.replace_extension(".flat_graph.nop");
}

std::filesystem::path map_to_hierarchical_graph_path(const std::filesystem::path &map_path) {
    // ./AR00011SR.map to ./AR00011SR.hierarchical_graph.nop
    std::filesystem::path hierarchical_graph_path = map_path;
    return hierarchical_graph_path.replace_extension(".hierarchical_graph.nop");
}

}    // namespace tpl_search
