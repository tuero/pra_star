// File: algorithm_types.h
// Enum class definition of algorithm types

#ifndef PRA_ALGORITHM_TYPES_H
#define PRA_ALGORITHM_TYPES_H

#include <string>
#include <unordered_map>

namespace tpl_search {

enum class AlgorithmType { AStar, PRAStar };

const std::unordered_map<std::string, AlgorithmType> ALGORITHM_STR_MAP{
    {"astar", AlgorithmType::AStar},
    {"pra", AlgorithmType::PRAStar},
};

}    // namespace tpl_search

#endif    // PRA_ALGORITHM_TYPES_H
