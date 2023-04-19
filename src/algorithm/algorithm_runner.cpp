// File: algorithm_runner.h
// Runner for setup and calling of search algorithms

#include "algorithm_runner.h"

#include <filesystem>
#include <fstream>
#include <iostream>

#include "algorithm/a_star/a_star.h"
#include "algorithm/common/graph_generator.h"
#include "algorithm/pra_star/pra_star.h"
#include "algorithm_types.h"
#include "util/file_util.h"
#include "util/map.h"
#include "util/scenario.h"

namespace tpl_search {

const std::string HEADER =
    "start_x,start_y,goal_x,goal_y,optimal_cost,solution_cost,expanded,generated,duration,first_move_duration";

void algorithm_runner_astar(const std::string &scenario_path, const std::vector<Scenario> &scenarios,
                            std::ofstream &export_file) {
    FlatGraph graph = load_flat_graph(scenario_to_map_path(scenario_path));
    export_file << HEADER << std::endl;

    for (const auto &scenario : scenarios) {
        SearchOutput output = a_star(graph, {scenario.start_x, scenario.start_y}, {scenario.goal_x, scenario.goal_y});
        std::cout << "Solution from (" << scenario.start_x << "," << scenario.start_y << "), to (" << scenario.goal_x
                  << "," << scenario.goal_y << "). Optimal cost: " << scenario.optimal_cost
                  << ", Found cost: " << output.path_cost << ", Expanded: " << output.expanded
                  << ", Generated: " << output.generated << ", Total duration: " << output.duration
                  << ", First move duration: " << output.first_move_duration << std::endl;
        export_file << scenario.start_x << "," << scenario.start_y << "," << scenario.goal_x << "," << scenario.goal_y
                    << "," << scenario.optimal_cost << "," << output.path_cost << "," << output.expanded << ","
                    << output.generated << "," << output.duration << "," << output.first_move_duration << std::endl;
    }
}

void algorithm_runner_pra(const std::string &scenario_path, const std::vector<Scenario> &scenarios, std::size_t k,
                          std::ofstream &export_file) {
    HierarchicalGraph graph = load_hierarchical_graph(scenario_to_map_path(scenario_path));
    export_file << HEADER << std::endl;

    for (const auto &scenario : scenarios) {
        SearchOutput output =
            pra_star(graph, k, {scenario.start_x, scenario.start_y}, {scenario.goal_x, scenario.goal_y});
        std::cout << "Solution from (" << scenario.start_x << "," << scenario.start_y << "), to (" << scenario.goal_x
                  << "," << scenario.goal_y << "). Optimal cost: " << scenario.optimal_cost
                  << ", Found cost: " << output.path_cost << ", Expanded: " << output.expanded
                  << ", Generated: " << output.generated << ", Total duration: " << output.duration
                  << ", First move duration: " << output.first_move_duration << std::endl;
        export_file << scenario.start_x << "," << scenario.start_y << "," << scenario.goal_x << "," << scenario.goal_y
                    << "," << scenario.optimal_cost << "," << output.path_cost << "," << output.expanded << ","
                    << output.generated << "," << output.duration << "," << output.first_move_duration << std::endl;
    }
}

void algorithm_runner(const std::string &scenario_path, const std::vector<Scenario> &scenarios,
                      const std::string &algorithm_str, std::size_t k, const std::string export_path) {
    // Ensure algorithm is known
    if (ALGORITHM_STR_MAP.find(algorithm_str) == ALGORITHM_STR_MAP.end()) {
        std::cerr << "Error: Unknown algorithm type." << std::endl;
        std::exit(1);
    }

    // Instantiate graph of underlying binary grid
    AlgorithmType algorithm = ALGORITHM_STR_MAP.at(algorithm_str);

    std::filesystem::create_directory(std::filesystem::path(export_path).parent_path());
    if (std::filesystem::exists(export_path)) {
        std::filesystem::remove(export_path);
    }
    std::ofstream export_file(export_path, std::ofstream::app | std::ofstream::out);

    switch (algorithm) {
        case AlgorithmType::AStar: {
            algorithm_runner_astar(scenario_path, scenarios, export_file);
            break;
        }
        case AlgorithmType::PRAStar: {
            algorithm_runner_pra(scenario_path, scenarios, k, export_file);
            break;
        }
        default:
            __builtin_unreachable();
    }

    export_file.close();
}

}    // namespace tpl_search
