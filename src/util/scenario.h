// File: scenario.h
// Scenario information

#ifndef PRA_UTIL_SCENARIO_H
#define PRA_UTIL_SCENARIO_H

#include <istream>
#include <ostream>
#include <string>
#include <vector>

namespace tpl_search {

// Scenario information
struct Scenario {
    friend std::istream &operator>>(std::istream &input_stream, Scenario &scenario);
    friend std::ostream &operator<<(std::ostream &output_stream, Scenario &scenario);

    int bucket;
    std::string map_name;
    std::size_t width;
    std::size_t height;
    std::size_t start_x;
    std::size_t start_y;
    std::size_t goal_x;
    std::size_t goal_y;
    double optimal_cost;
};

/**
 * Load a single scenario
 * @param scenario_file_path Path to load scenario
 * @param N The scenario number in the file
 * @return The scenario
 */
Scenario load_scenario(const std::string &scenario_file_path, std::size_t N);

/**
 * Load all scenarios from a file
 * @param scenario_file_path Path to load scenario
 * @return The scenario
 */
std::vector<Scenario> load_scenarios(const std::string &scenario_file_path);

}    // namespace tpl_search

#endif    // PRA_UTIL_SCENARIO_H
