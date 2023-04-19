// File: test_astar.cpp
// Test A* on known optimal paths

#include <filesystem>
#include <iostream>

#include "algorithm/a_star/a_star.h"
#include "algorithm/common/graph_generator.h"
#include "algorithm/common/graph_util.h"
#include "test_macros.h"
#include "util/file_util.h"
#include "util/scenario.h"
#include "util/timer.h"

using namespace tpl_search;

int main() {
    std::filesystem::path scenario_path(__FILE__);
    scenario_path = scenario_path.replace_filename("battleground.map.scen");
    {
        Scenario scenario = load_scenario(scenario_path, 0);
        FlatGraph graph = load_flat_graph(scenario_to_map_path(scenario_path));
        SearchOutput search_output =
            a_star(graph, {scenario.start_x, scenario.start_y}, {scenario.goal_x, scenario.goal_y});
        REQUIRE_NEAR(search_output.path_cost, scenario.optimal_cost, 1e-5);
    }
    {
        Scenario scenario = load_scenario(scenario_path, 1328);
        FlatGraph graph = load_flat_graph(scenario_to_map_path(scenario_path));
        SearchOutput search_output =
            a_star(graph, {scenario.start_x, scenario.start_y}, {scenario.goal_x, scenario.goal_y});
        REQUIRE_NEAR(search_output.path_cost, scenario.optimal_cost, 1e-5);
    }
    {
        Scenario scenario = load_scenario(scenario_path, 1328);
        FlatGraph graph_original = load_flat_graph(scenario_to_map_path(scenario_path));
        HierarchicalGraph::ParentChildMap parent_child_map;
        ThreadTimer timer1;
        timer1.start();
        FlatGraph graph = create_abstract_graph(graph_original, parent_child_map);
        std::cout << timer1.get_duration() << std::endl;

        parent_child_map.clear();
        ThreadTimer timer2;
        timer2.start();
        graph = create_abstract_graph(graph, parent_child_map);
        std::cout << timer2.get_duration() << std::endl;

        SearchOutput search_output =
            a_star(graph, {scenario.start_x, scenario.start_y}, {scenario.goal_x, scenario.goal_y});
        // REQUIRE_NEAR(search_output.path_cost, scenario.optimal_cost, 1e-5);
    }
}
