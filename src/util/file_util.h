// File: file_util.h
// File utility

#ifndef PRA_UTIL_FILE_H
#define PRA_UTIL_FILE_H

#include <filesystem>
#include <string>

namespace tpl_search {

/**
 * Open and validate a file
 * @param file_path Path of file to open
 * @return File stream to open file
 */
std::ifstream open_and_validate_file(const std::string &file_path);

/**
 * Check whether input stream is still valid after read
 * @param message Error message
 */
void check_input_stream(const std::string &message);

/**
 * Convert scenario path to map path
 * @param file_path Path of file to open
 * @return File path to corresponding map
 */
std::filesystem::path scenario_to_map_path(const std::filesystem::path &scenario_path);

/**
 * Convert map path to flat graph path
 * @param map_path Path of map file
 * @return File path to corresponding flat graph
 */
std::filesystem::path map_to_flat_graph_path(const std::filesystem::path &map_path);

/**
 * Convert map path to hierarchical graph path
 * @param map_path Path of map file
 * @return File path to corresponding hierarchical graph
 */
std::filesystem::path map_to_hierarchical_graph_path(const std::filesystem::path &map_path);

}    // namespace tpl_search

#endif    // PRA_UTIL_FILE_H
