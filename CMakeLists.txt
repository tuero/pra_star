cmake_minimum_required (VERSION 3.12)

project(pra_star)

# Set flags
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(ABSL_PROPAGATE_CXX_STD ON)


# Build abseil and include for linking for all child targets
add_subdirectory(external/abseil-cpp)
link_libraries( 
    absl::flags
    absl::flags_parse
)

# Let child targets include from external libraries not needed to be built as libraries
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/libnop/include)

# Build child targets
add_subdirectory(src)
add_subdirectory(tests)
