// File: test_scenario_loader.cpp
// Test scenario loading and data extracting from files

#include <filesystem>
#include <iostream>

#include "test_macros.h"
#include "util/file_util.h"
#include "util/map.h"
#include "util/scenario.h"

using namespace tpl_search;

int main() {
    std::filesystem::path scenario_path(__FILE__);
    {
        scenario_path = scenario_path.replace_filename("battleground.map.scen");
        Scenario scenario = load_scenario(scenario_path, 0);
        std::filesystem::path map_path = scenario_to_map_path(scenario_path);
        Map map = load_map(map_path);

        REQUIRE_TRUE(scenario.map_name == "battleground.map");
        REQUIRE_TRUE(scenario.width == 512);
        REQUIRE_TRUE(scenario.height == 512);
        REQUIRE_TRUE(scenario.start_x == 146);
        REQUIRE_TRUE(scenario.start_y == 393);
        REQUIRE_TRUE(scenario.goal_x == 148);
        REQUIRE_TRUE(scenario.goal_y == 394);
        REQUIRE_NEAR(scenario.optimal_cost, 2.41421356, 1e-5);
        REQUIRE_TRUE(map.grid_map[scenario.start_y * scenario.width + scenario.start_x]);
        REQUIRE_TRUE(map.grid_map[scenario.goal_y * scenario.width + scenario.goal_x]);
    }
    {
        scenario_path = scenario_path.replace_filename("battleground.map.scen");
        Scenario scenario = load_scenario(scenario_path, 1328);
        std::filesystem::path map_path = scenario_to_map_path(scenario_path);
        Map map = load_map(map_path);

        REQUIRE_TRUE(scenario.map_name == "battleground.map");
        REQUIRE_TRUE(scenario.width == 512);
        REQUIRE_TRUE(scenario.height == 512);
        REQUIRE_TRUE(scenario.start_x == 396);
        REQUIRE_TRUE(scenario.start_y == 436);
        REQUIRE_TRUE(scenario.goal_x == 57);
        REQUIRE_TRUE(scenario.goal_y == 77);
        REQUIRE_NEAR(scenario.optimal_cost, 531.63665151, 1e-5);
        REQUIRE_TRUE(map.grid_map[scenario.start_y * scenario.width + scenario.start_x]);
        REQUIRE_TRUE(map.grid_map[scenario.goal_y * scenario.width + scenario.goal_x]);
    }
}
