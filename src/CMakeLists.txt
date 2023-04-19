# Source files
set(COMMON_SOURCES
    algorithm/a_star/a_star.cpp
    algorithm/common/graph.cpp
    algorithm/common/graph_util.cpp
    algorithm/common/graph_generator.cpp
    algorithm/pra_star/pra_star.cpp
    algorithm/algorithm_runner.cpp
    util/file_util.cpp
    util/map.cpp
    util/scenario.cpp
)

add_library(pra_star_common STATIC ${COMMON_SOURCES})
target_compile_options(pra_star_common PUBLIC 
    -Wall -Wextra
    $<$<CONFIG:RELEASE>:-O3> $<$<CONFIG:RELEASE>:-DNDEBUG>
    $<$<CONFIG:DEBUG>:-g> $<$<CONFIG:DEBUG>:-O0> $<$<CONFIG:DEBUG>:-DDEBUG> $<$<CONFIG:DEBUG>:-pg> $<$<CONFIG:DEBUG>:-Wconversion> 
    $<$<CONFIG:DEBUG>:-Wsign-conversion>  $<$<CONFIG:DEBUG>:-fsanitize=address>
)
target_link_options(pra_star_common PUBLIC 
    $<$<CONFIG:DEBUG>:-fsanitize=address>
    $<$<CONFIG:DEBUG>:-pg>
)

# Main entry point for running algorithms on single instance
add_executable(run_single run_single.cpp)
target_link_libraries(run_single PUBLIC pra_star_common)

# Main entry point for running algorithms on all scenario instances
add_executable(run_multi run_multi.cpp)
target_link_libraries(run_multi PUBLIC pra_star_common)

# Main entry point for creating and exporting the graphs
add_executable(create_graphs create_graphs.cpp)
target_link_libraries(create_graphs PUBLIC pra_star_common)

include_directories(.)
