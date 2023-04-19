// File: run_single.cpp
// Main entry point for running a single scenario

#include <absl/flags/flag.h>
#include <absl/flags/parse.h>
#include <absl/flags/usage.h>
#include <absl/strings/str_cat.h>

#include "algorithm/algorithm_runner.h"

ABSL_FLAG(std::string, scenario_path, "/opt/", "Full path for the scenario");
ABSL_FLAG(std::size_t, scenario_number, 0, "Scenario number for selecting from the scenario path");
ABSL_FLAG(std::string, algorithm, "pra_star", "Search algorithm to run");
ABSL_FLAG(std::size_t, k, 0, "K parameter for PRA*, use 0 as infinity");
ABSL_FLAG(std::string, export_path, "/opt/", "Base directory for saved metrics");

using namespace tpl_search;

int main(int argc, char** argv) {
    absl::SetProgramUsageMessage(
        absl::StrCat("Usage:\n", argv[0], " <scenario_path> <scenario_number> <export_path> <algorithm> <k>"));
    absl::ParseCommandLine(argc, argv);
    std::string scenario_path = absl::GetFlag(FLAGS_scenario_path);
    std::size_t scenario_number = absl::GetFlag(FLAGS_scenario_number);
    std::string algorithm = absl::GetFlag(FLAGS_algorithm);
    std::size_t k = absl::GetFlag(FLAGS_k);
    std::string export_path = absl::GetFlag(FLAGS_export_path);

    Scenario scenario = load_scenario(scenario_path, scenario_number);
    algorithm_runner(scenario_path, {scenario}, algorithm, k, export_path);
}
