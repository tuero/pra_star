// File: run_multi.cpp
// Main entry point for running all scenarios for a given file

#include <absl/flags/flag.h>
#include <absl/flags/parse.h>
#include <absl/flags/usage.h>
#include <absl/strings/str_cat.h>

#include "algorithm/algorithm_runner.h"

ABSL_FLAG(std::string, scenario_path, "/opt/", "Full path for the scenario");
ABSL_FLAG(std::string, algorithm, "pra_star", "Search algorithm to run");
ABSL_FLAG(std::size_t, k, 0, "K parameter for PRA*, use 0 as infinity");
ABSL_FLAG(std::string, export_path, "/opt/", "Base directory for saved metrics");

using namespace tpl_search;

int main(int argc, char** argv) {
    absl::SetProgramUsageMessage(absl::StrCat("Usage:\n", argv[0], " <scenario_path> <export_path> <algorithm> <k>"));
    absl::ParseCommandLine(argc, argv);
    std::string scenario_path = absl::GetFlag(FLAGS_scenario_path);
    std::string algorithm = absl::GetFlag(FLAGS_algorithm);
    std::size_t k = absl::GetFlag(FLAGS_k);
    std::string export_path = absl::GetFlag(FLAGS_export_path);

    std::vector<Scenario> scenarios = load_scenarios(scenario_path);
    algorithm_runner(scenario_path, scenarios, algorithm, k, export_path);
}
