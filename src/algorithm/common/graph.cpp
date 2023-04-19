// File: graph.cpp
// Graph structs used in search

#include "graph.h"

#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>

#include "graph_util.h"

namespace tpl_search {

const double SQRT2 = std::sqrt(2.0);

double distance(const GridPosition &p1, const GridPosition &p2) {
    double adx = std::abs(static_cast<double>(p1.x) - static_cast<double>(p2.x));
    double ady = std::abs(static_cast<double>(p1.y) - static_cast<double>(p2.y));
    return SQRT2 * std::min(adx, ady) + std::abs(adx - ady);
}

double distance(const AbstractPosition &p1, const AbstractPosition &p2) {
    double adx = std::abs(p1.x - p2.x);
    double ady = std::abs(p1.y - p2.y);
    return SQRT2 * std::min(adx, ady) + std::abs(adx - ady);
}

double distance(const AbstractPosition &p1, const GridPosition &p2) {
    double adx = std::abs(p1.x - static_cast<double>(p2.x));
    double ady = std::abs(p1.y - static_cast<double>(p2.y));
    return SQRT2 * std::min(adx, ady) + std::abs(adx - ady);
}

double distance(const GraphNode *n1, const GraphNode *n2) {
    double adx = std::abs(n1->position.x - n2->position.x);
    double ady = std::abs(n1->position.y - n2->position.y);
    return SQRT2 * std::min(adx, ady) + std::abs(adx - ady);
}

// -------------------------- FlatGraph  --------------------------

void FlatGraph::add_node(const GraphNode &node) {
    node_id_idx_map[node.id] = node_storage.size();
    neighbour_mapping[node.id].clear();
    node_storage.push_back(node);
    for (const auto &position : node.represented_positions) {
        position_id_mapping[position] = node.id;
    }
}

void FlatGraph::add_edge(std::size_t id1, std::size_t id2) {
    assert(node_id_idx_map.find(id1) != node_id_idx_map.end());
    assert(node_id_idx_map.find(id2) != node_id_idx_map.end());
    neighbour_mapping[id1].insert(id2);
    neighbour_mapping[id2].insert(id1);
    ++edge_counter;
}

std::size_t FlatGraph::get_edge_count() {
    return edge_counter;
}

void FlatGraph::set_constrained_nodes(const std::unordered_set<std::size_t> &node_ids) {
    constrained_nodes = node_ids;
}

const GraphNode *FlatGraph::get_node(std::size_t id) const {
    return &node_storage.at(node_id_idx_map.at(id));
}

std::size_t FlatGraph::get_pos_node_id(const GridPosition &position) const {
    assert(position_id_mapping.find(position) != position_id_mapping.end());
    return position_id_mapping.at(position);
}

const std::vector<GraphNode> &FlatGraph::get_all_nodes() const {
    return node_storage;
}

std::vector<std::size_t> FlatGraph::get_all_node_ids() const {
    std::vector<std::size_t> node_ids;
    node_ids.reserve(node_storage.size());
    for (auto const &node : node_storage) {
        node_ids.push_back(node.id);
    }
    return node_ids;
}

bool FlatGraph::are_neighbours(std::size_t node_id1, std::size_t node_id2) const {
    return neighbour_mapping.at(node_id1).find(node_id2) != neighbour_mapping.at(node_id1).end();
}

void FlatGraph::get_neighbours(std::size_t node_id, std::vector<std::size_t> &neighbour_ids) const {
    neighbour_ids.clear();
    for (auto const &id : neighbour_mapping.at(node_id)) {
        if (constrained_nodes.empty() || constrained_nodes.find(node_id) != constrained_nodes.end()) {
            neighbour_ids.push_back(id);
        }
    }
}

std::vector<std::size_t> FlatGraph::get_neighbours(std::size_t node_id) const {
    std::vector<std::size_t> neighbour_ids;
    for (auto const &id : neighbour_mapping.at(node_id)) {
        neighbour_ids.push_back(id);
    }
    return neighbour_ids;
}

std::size_t FlatGraph::get_node_degree(std::size_t node_id) const {
    return neighbour_mapping.at(node_id).size();
}

void FlatGraph::save(const std::string &path) const {
    if (!std::filesystem::exists(std::filesystem::path(path).parent_path())) {
        std::filesystem::create_directories(std::filesystem::path(path).parent_path());
    }
    std::filesystem::remove(path);
    std::cout << "Exporting FlatGraph to " << path << std::endl;
    nop::Serializer<nop::StreamWriter<std::ofstream>> serializer{path};

    save(serializer);
}

void FlatGraph::save(nop::Serializer<nop::StreamWriter<std::ofstream>> &serializer) const {
    // Necessary conversions due to libnop not supporting unordered_set
    std::vector<GraphNode::GraphNodeSerialize> nodes_serializable;
    nodes_serializable.reserve(node_storage.size());
    for (const auto &node : node_storage) {
        nodes_serializable.emplace_back(GraphNode::GraphNodeSerialize::from_node(node));
    }

    std::unordered_map<std::size_t, std::vector<std::size_t>> neighbour_mapping_serializable;
    for (const auto &[node, neighbours] : neighbour_mapping) {
        neighbour_mapping_serializable[node] = std::vector<std::size_t>(neighbours.begin(), neighbours.end());
    }

    std::vector<std::size_t> constrained_nodes_serializable(constrained_nodes.begin(), constrained_nodes.end());

    serializer.Write(nodes_serializable);
    serializer.Write(this->node_id_idx_map);
    serializer.Write(neighbour_mapping_serializable);
    serializer.Write(this->position_id_mapping);
    serializer.Write(constrained_nodes_serializable);
    serializer.Write(this->edge_counter);
}

void FlatGraph::load(const std::string &path) {
    if (!std::filesystem::exists(path)) {
        std::cerr << "Error: " << path << " does not exist." << std::endl;
        exit(1);
    }
    std::cout << "Loading FlatGraph from " << path << std::endl;
    nop::Deserializer<nop::StreamReader<std::ifstream>> deserializer{path};

    load(deserializer);
}

void FlatGraph::load(nop::Deserializer<nop::StreamReader<std::ifstream>> &deserializer) {
    std::vector<GraphNode::GraphNodeSerialize> nodes_serializable;
    std::unordered_map<std::size_t, std::vector<std::size_t>> neighbour_mapping_serializable;
    std::vector<std::size_t> constrained_nodes_serializable(constrained_nodes.begin(), constrained_nodes.end());

    deserializer.Read(&(nodes_serializable));
    deserializer.Read(&(this->node_id_idx_map));
    deserializer.Read(&(neighbour_mapping_serializable));
    deserializer.Read(&(this->position_id_mapping));
    deserializer.Read(&(constrained_nodes_serializable));
    deserializer.Read(&(this->edge_counter));

    // Necessary conversions due to libnop not supporting unordered_set
    node_storage.clear();
    node_storage.reserve(node_storage.size());
    for (const auto &node : nodes_serializable) {
        node_storage.emplace_back(GraphNode::from_serializable(node));
    }

    neighbour_mapping.clear();
    for (const auto &[node, neighbours] : neighbour_mapping_serializable) {
        neighbour_mapping[node] = std::unordered_set<std::size_t>(neighbours.begin(), neighbours.end());
    }

    constrained_nodes.clear();
    constrained_nodes.insert(constrained_nodes_serializable.begin(), constrained_nodes_serializable.end());
}

// -------------------------- FlatGraph  --------------------------

// -------------------------- HierarchicalGraph  --------------------------

HierarchicalGraph::HierarchicalGraph(const FlatGraph &graph) {
    flat_graph_layers.push_back(graph);

    while (flat_graph_layers.back().get_all_node_ids().size() > 1 && flat_graph_layers.back().get_edge_count() > 0) {
        std::cout << "Building layer " << flat_graph_layers.size() << std::endl;
        ParentChildMap parent_child_mapping;
        FlatGraph abstract_graph = create_abstract_graph(flat_graph_layers.back(), parent_child_mapping);
        flat_graph_layers.push_back(abstract_graph);
        parent_child_mappings.push_back(parent_child_mapping);
        std::cout << "Built layer " << flat_graph_layers.size() - 1 << " with nodes "
                  << flat_graph_layers.back().get_all_node_ids().size() << " edges "
                  << flat_graph_layers.back().get_edge_count() << std::endl;
    }
}

std::size_t HierarchicalGraph::num_layers() const {
    return flat_graph_layers.size();
}

FlatGraph &HierarchicalGraph::get_layer(std::size_t layer_idx) {
    assert(layer_idx < flat_graph_layers.size());
    return flat_graph_layers.at(layer_idx);
}

const std::unordered_set<std::size_t> &HierarchicalGraph::get_parent_child_mapping(std::size_t level,
                                                                                   std::size_t parent_node_id) {
    assert(level < parent_child_mappings.size());
    assert(parent_child_mappings[level].find(parent_node_id) != parent_child_mappings[level].end());
    return parent_child_mappings.at(level).at(parent_node_id);
}

void HierarchicalGraph::save(const std::string &path) const {
    if (!std::filesystem::exists(std::filesystem::path(path).parent_path())) {
        std::filesystem::create_directories(std::filesystem::path(path).parent_path());
    }
    std::filesystem::remove(path);
    std::cout << "Exporting HierarchicalGraph to " << path << std::endl;
    nop::Serializer<nop::StreamWriter<std::ofstream>> serializer{path};

    serializer.Write(flat_graph_layers.size());
    for (const auto &flat_graph : flat_graph_layers) {
        flat_graph.save(serializer);
    }

    std::vector<ParentChildMapSerializable> parent_child_mappings_serializable;
    for (const auto &parent_child_mapping : parent_child_mappings) {
        ParentChildMapSerializable map;
        for (const auto &[parent, children] : parent_child_mapping) {
            map[parent] = std::vector<std::size_t>(children.begin(), children.end());
        }
        parent_child_mappings_serializable.emplace_back(std::move(map));
    }
    serializer.Write(parent_child_mappings_serializable);
}

void HierarchicalGraph::load(const std::string &path) {
    if (!std::filesystem::exists(path)) {
        std::cerr << "Error: " << path << " does not exist." << std::endl;
        exit(1);
    }
    std::cout << "Loading HierarchicalGraph from " << path << std::endl;
    nop::Deserializer<nop::StreamReader<std::ifstream>> deserializer{path};

    std::size_t flat_graph_layers_size = 0;
    deserializer.Read(&(flat_graph_layers_size));
    flat_graph_layers.clear();
    flat_graph_layers.reserve(flat_graph_layers_size);
    for (std::size_t i = 0; i < flat_graph_layers_size; ++i) {
        FlatGraph flat_graph;
        flat_graph.load(deserializer);
        flat_graph_layers.emplace_back(std::move(flat_graph));
    }

    std::vector<ParentChildMapSerializable> parent_child_mappings_serializable;
    deserializer.Read(&(parent_child_mappings_serializable));
    parent_child_mappings.clear();
    parent_child_mappings.reserve(parent_child_mappings_serializable.size());
    for (const auto &parent_child_mapping : parent_child_mappings_serializable) {
        ParentChildMap map;
        for (const auto &[parent, children] : parent_child_mapping) {
            map[parent] = std::unordered_set<std::size_t>(children.begin(), children.end());
        }
        parent_child_mappings.emplace_back(std::move(map));
    }
}

// -------------------------- HierarchicalGraph  --------------------------

}    // namespace tpl_search
