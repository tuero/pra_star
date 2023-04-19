// File: scenario.cpp
// Scenario information

#include "scenario.h"

#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>

#include "file_util.h"

namespace tpl_search {

std::istream &operator>>(std::istream &input_stream, Scenario &scenario) {
    input_stream >> scenario.bucket >> scenario.map_name >> scenario.width >> scenario.height >> scenario.start_x >>
        scenario.start_y >> scenario.goal_x >> scenario.goal_y >> scenario.optimal_cost;
    if (input_stream.fail()) {
        std::cerr << "Error: Line is not in correct format. Expected [bucket(int) map_name(str) width(int) height(int) "
                     "start_x(int) start_y(int) goal_x(int) goal_y(int) optimal_cost(float)]"
                  << std::endl;
        std::exit(1);
    }
    return input_stream;
}

std::ostream &operator<<(std::ostream &output_stream, Scenario &scenario) {
    output_stream << "Map name: " << scenario.map_name << ", width: " << scenario.width
                  << ", height: " << scenario.height << ", start: (" << scenario.start_x << ", " << scenario.start_y
                  << "), goal: (" << scenario.goal_x << ", " << scenario.goal_y
                  << "), optimal cost: " << scenario.optimal_cost;
    return output_stream;
}

Scenario load_scenario(const std::string &scenario_file_path, std::size_t N) {
    std::ifstream scenario_file = open_and_validate_file(scenario_file_path);

    // Get Nth problem instance
    // We always skip first line which holds the version number
    std::string line;
    for (std::size_t i = 0; i <= N + 1; ++i) {
        std::getline(scenario_file, line);
        check_input_stream("line " + std::to_string(i + 1));
    }

#ifdef DEBUG
    std::cout << "Debug: Using scenario " << line << std::endl;
#endif

    // line is set to last line read which is the line we are looking for
    std::istringstream iss(line);
    Scenario scenario;
    iss >> scenario;

#ifdef DEBUG
    std::cout << scenario << std::endl;
#endif
    return scenario;
}

std::vector<Scenario> load_scenarios(const std::string &scenario_file_path) {
    std::vector<Scenario> scenarios;
    std::ifstream scenario_file = open_and_validate_file(scenario_file_path);

    // We always skip first line which holds the version number
    std::string line;
    std::getline(scenario_file, line);
    while (std::getline(scenario_file, line)) {
        std::istringstream iss(line);
        Scenario scenario;
        iss >> scenario;
        scenarios.push_back(scenario);
    }

    return scenarios;
}

}    // namespace tpl_search
