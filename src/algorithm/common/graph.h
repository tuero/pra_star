// File: graph.h
// Graph structs used in search

#ifndef PRA_ALGORITHM_COMMON_GRAPH_H
#define PRA_ALGORITHM_COMMON_GRAPH_H

#include <absl/container/flat_hash_set.h>
#include <nop/serializer.h>
#include <nop/structure.h>
#include <nop/utility/stream_reader.h>
#include <nop/utility/stream_writer.h>

#include <memory>
#include <ostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace tpl_search {

// Types of positions
template <typename T>
struct Position {
    T x;
    T y;
    NOP_STRUCTURE(Position, x, y);
};

template <typename T>
bool operator==(const Position<T> &lhs, const Position<T> &rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

template <typename T>
bool operator!=(const Position<T> &lhs, const Position<T> &rhs) {
    return !(lhs == rhs);
}

// Grid vs abstract positions
using GridPosition = Position<std::size_t>;
using AbstractPosition = Position<double>;

struct PairHash {
    std::size_t operator()(const GridPosition &p) const {
        return (p.x << 16) | p.y;
    }
};

template <typename T>
std::ostream &operator<<(std::ostream &os, const Position<T> &p) {
    os << "(" << p.x << ", " << p.y << ")";

    return os;
}

// Node used in graph search
struct GraphNode {
    // Specialization for serializing
    struct GraphNodeSerialize {
        // Convert to regular graph node
        static GraphNodeSerialize from_node(const GraphNode &node) {
            return {
                node.id, node.position,
                decltype(represented_positions)(node.represented_positions.begin(), node.represented_positions.end())};
        }

        std::size_t id;
        AbstractPosition position;
        std::vector<GridPosition> represented_positions;
        NOP_STRUCTURE(GraphNodeSerialize, id, position, represented_positions);
    };

    // Convert to serializable graph node
    static GraphNode from_serializable(const GraphNodeSerialize &node) {
        return {node.id, node.position,
                decltype(represented_positions)(node.represented_positions.begin(), node.represented_positions.end())};
    }

    std::size_t id;
    AbstractPosition position;
    std::unordered_set<GridPosition, PairHash> represented_positions;

    struct Hasher {
        std::size_t operator()(const GraphNode &node) const {
            return node.id;
        }
    };

    bool operator==(const GraphNode &other) const {
        return id == other.id;
    }
};

/**
 * Compute distance between two positions
 * @param p1 Grid position of first point
 * @param p2 Grid position of second point
 * @return distance between the two points
 */
double distance(const GridPosition &p1, const GridPosition &p2);

/**
 * Compute distance between two positions
 * @param p1 Abstract position of first point
 * @param p2 Abstract position of second point
 * @return distance between the two points
 */
double distance(const AbstractPosition &p1, const AbstractPosition &p2);

/**
 * Compute distance between two positions
 * @param p1 Abstract position of first point
 * @param p2 Grid position of second point
 * @return distance between the two points
 */
double distance(const AbstractPosition &p1, const GridPosition &p2);

/**
 * Compute distance between graph nodes
 * @param n1 First graph node
 * @param n2 Second graph node
 * @return distance between the two graph nodes
 */
double distance(const GraphNode *n1, const GraphNode *n2);

// Flat graph used in search
class FlatGraph {
public:
    FlatGraph() = default;

    /**
     * Set the constrained nodes to be used in search
     * @param node_ids Node IDs of restricted set
     */
    void set_constrained_nodes(const std::unordered_set<std::size_t> &node_ids = {});

    /**
     * Add node to the graph
     * @param node Node to add
     */
    void add_node(const GraphNode &node);

    /**
     * Add edge between two nodes
     * @note This does a undirected edge between both nodes
     * @param id1 Node ID of first node
     * @param id2 Node ID of second node
     */
    void add_edge(std::size_t id1, std::size_t id2);

    /**
     * Get the edge count
     * @return Number of edges in the graph
     */
    std::size_t get_edge_count();

    /**
     * Get a pointer to a node
     * @param id Node ID to query
     * @return Pointer to node matching that ID
     */
    const GraphNode *get_node(std::size_t id) const;

    /**
     * Get the node ID a position is represented by
     * @param position The grid position to query
     * @return Node ID represetning that position
     */
    std::size_t get_pos_node_id(const GridPosition &position) const;

    /**
     * Get all nodes in the graph
     * @return Vector of all nodes in the graph
     */
    const std::vector<GraphNode> &get_all_nodes() const;

    /**
     * Get all node IDs in the graph
     * @return Vector of all node IDs in the graph
     */
    std::vector<std::size_t> get_all_node_ids() const;

    /**
     * Check if two nodes are neighbours
     * @param node_id1 ID of the first node
     * @param node_id2 ID of the second node
     * @return True if nodes are neighbours, false otherwise
     */
    bool are_neighbours(std::size_t node_id1, std::size_t node_id2) const;

    /**
     * Get all neighbour IDs of a given node
     * @param node_id ID to query
     * @param neighbour_ids Storage to place the neighbours in
     */
    void get_neighbours(std::size_t node_id, std::vector<std::size_t> &neighbour_ids) const;

    /**
     * Get all neighbour IDs of a given node
     * @param node_id ID to query
     * @return Vector of node IDs of the neighbours
     */
    std::vector<std::size_t> get_neighbours(std::size_t node_id) const;

    /**
     * Get the degree of a node
     * @param node_id ID to query
     * @return Degree of that node
     */
    std::size_t get_node_degree(std::size_t node_id) const;

    /**
     * Save the graph to the given path
     * @param path Path to serialize the graph
     */
    void save(const std::string &path) const;

    /**
     * Save the graph to the given stream writer
     * @param serializer Serializer object to place the graph in
     */
    void save(nop::Serializer<nop::StreamWriter<std::ofstream>> &serializer) const;

    /**
     * Load the graph from a given path
     * @param path Path to load the graph from
     */
    void load(const std::string &path);

    /**
     * Load the graph from a given deserializer object
     * @param deserializer Deserializer object to load from
     */
    void load(nop::Deserializer<nop::StreamReader<std::ifstream>> &deserializer);

private:
    std::vector<GraphNode> node_storage;
    std::unordered_map<std::size_t, std::size_t> node_id_idx_map;
    std::unordered_map<std::size_t, std::unordered_set<std::size_t>> neighbour_mapping;
    std::unordered_map<GridPosition, std::size_t, PairHash> position_id_mapping;
    std::unordered_set<std::size_t> constrained_nodes;
    std::size_t edge_counter = 0;
};

// Hierarchical graph composed of flat layer graphs
class HierarchicalGraph {
public:
    using ParentChildMap = std::unordered_map<std::size_t, std::unordered_set<std::size_t>>;
    using ParentChildMapSerializable = std::unordered_map<std::size_t, std::vector<std::size_t>>;

    HierarchicalGraph() = default;
    HierarchicalGraph(const FlatGraph &graph);

    /**
     * Get the number of layers in the hierarchical graph
     * @return Number of layers the graph represents
     */
    std::size_t num_layers() const;

    /**
     * Get a particular layer
     * @param layer_idx Layer index to query
     * @return Reference to the flat layer for that index
     */
    FlatGraph &get_layer(std::size_t layer_idx);

    /**
     * Get the mapping of child nodes to a given parent node
     * @param level Level to query for
     * @param parent_node_id The node ID of the parent
     * @return Set of children node IDs represented by the parent in the layer below
     */
    const std::unordered_set<std::size_t> &get_parent_child_mapping(std::size_t level, std::size_t parent_node_id);

    /**
     * Save the graph to the given path
     * @param path Path to serialize the graph
     */
    void save(const std::string &path) const;

    /**
     * Load the graph from a given path
     * @param path Path to load the graph from
     */
    void load(const std::string &path);

private:
    std::vector<FlatGraph> flat_graph_layers;
    std::vector<ParentChildMap> parent_child_mappings;
};

}    // namespace tpl_search

#endif    // PRA_ALGORITHM_COMMON_GRAPH_H
