// File: algorithm_runner.h
// Runner for setup and calling of search algorithms

#ifndef PRA_ALGORITHM_RUNNER_H
#define PRA_ALGORITHM_RUNNER_H

#include <string>
#include <vector>

#include "util/scenario.h"

namespace tpl_search {

/**
 * Main algorithm runner
 * @param scenario_path Base path for scenario
 * @param scenarios Scenarios to solve
 * @param algorithm_str String algorithm name
 * @param k K parameter for PRA* truncation
 * @param export_path Path to save search results
 */
void algorithm_runner(const std::string &scenario_path, const std::vector<Scenario> &scenarios,
                      const std::string &algorithm_str, std::size_t k, const std::string export_path);

}    // namespace tpl_search

#endif    // PRA_ALGORITHM_RUNNER_H
