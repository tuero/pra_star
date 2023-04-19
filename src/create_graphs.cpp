// File: create_abstract_graph.cpp
// Main entry point

#include <absl/flags/flag.h>
#include <absl/flags/parse.h>
#include <absl/flags/usage.h>
#include <absl/strings/str_cat.h>

#include "algorithm/common/graph_generator.h"
#include "util/file_util.h"

ABSL_FLAG(std::string, map_path, "/opt/", "Full path for the map");

using namespace tpl_search;

int main(int argc, char** argv) {
    absl::SetProgramUsageMessage(absl::StrCat("Usage:\n", argv[0], " <map_path>"));
    absl::ParseCommandLine(argc, argv);
    std::string map_path = absl::GetFlag(FLAGS_map_path);

    FlatGraph flat_graph = load_flat_graph(map_path, true);
    flat_graph.save(map_to_flat_graph_path(map_path));

    HierarchicalGraph hierarchical_graph = load_hierarchical_graph(map_path, true);
    hierarchical_graph.save(map_to_hierarchical_graph_path(map_path));
}
