// File: priority_queue.h
// Indexed tracked priority queue

#ifndef PRA_UTIL_PQ_H
#define PRA_UTIL_PQ_H

#include <cassert>
#include <cstdint>
#include <unordered_map>
#include <vector>

namespace tpl_search {

// Priority set with index tracking for random access removal and updating
template <typename T, typename CompareT, typename HashT>
class PrioritySet {
public:
    PrioritySet() = default;

    bool operator==(const PrioritySet &other) const = default;

    /**
     * Insert the value.
     * @note If the value already exists, then no change occurs
     * @param t The value to insert
     */
    void insert(const T &t) {
        if (contains(t)) {
            return;
        }
        data.push_back(t);
        indices[hasher(data.back())] = size() - 1;
        swim(size() - 1);
    }

    /**
     * Insert the value.
     * @note If the value already exists, then no change occurs
     * @param t The value to insert
     */
    void insert(T &&t) {
        if (contains(t)) {
            return;
        }
        data.emplace_back(std::move(t));
        indices[hasher(data.back())] = size() - 1;
        swim(size() - 1);
    }

    /**
     * Removes the top element
     * @note If the priority set is empty, then no change occurs
     */
    void pop() {
        if (empty()) {
            return;
        }
        swap_elements(0, size() - 1);
        indices.erase(hasher(data.back()));
        data.pop_back();
        sink(0);
    }

    /**
     * Removes the top element
     * @note If the priority set is empty, then no change occurs
     * @param t The value to remove
     */
    void erase(const T &t) {
        if (!contains(t)) {
            return;
        }
        std::size_t idx = indices.at(hasher(t));
        swap_elements(idx, size() - 1);
        indices.erase(hasher(data.back()));
        data.pop_back();
        swim(idx);
        sink(idx);
    }

    /**
     * Get a reference to the top element
     * @note Since the reference is non-const, modifying the underlying element could lead to the priority set being in
     * an undetermined state
     * @return The top element
     */
    T &top() {
        return data.front();
    }

    /**
     * Get a const reference to the top element
     * @return The top element
     */
    const T &top() const {
        return data.front();
    }

    /**
     * Get a reference to the element with the corresponding hash
     * @note Since the reference is non-const, modifying the underlying element could lead to the priority set being in
     * an undetermined state
     * @param hash The hash corresponding to the element to search for
     * @return The top element
     */
    T &get(uint64_t hash) {
        assert(has_hash(hash));
        return data[indices.at(hash)];
    }

    /**
     * Get a reference to the element with the corresponding hash
     * @param hash The hash corresponding to the element to search for
     * @return The top element
     */
    const T &get(uint64_t hash) const {
        assert(has_hash(hash));
        return data[indices.at(hash)];
    }

    /**
     * Check if a element exists in the priority set
     * @return True if the element is contained in the priority set, false otherwise
     */
    bool contains(const T &t) const {
        return indices.find(hasher(t)) != indices.end();
    }

    /**
     * Update the element's priority
     * @note If the value already exists, then no change occurs
     * @note The element to update must share the same hash as the new element being passed
     * @param T The element to replace the existing element which shares the same hash
     */
    void update(const T &t) {
        if (!contains(t)) {
            return;
        }
        std::size_t idx = indices.at(hasher(t));
        data[idx] = t;
        swim(idx);
        sink(idx);
    }

    /**
     * Remove all elements from the priority set
     */
    void clear() {
        data.clear();
        indices.clear();
    }

    /**
     * Check if the priority set is empty
     * @return True if the priority set is empty, false otherwise
     */
    bool empty() const {
        return data.empty();
    }

    /**
     * Get the number of elements stored in the priority set
     * @return The number of elements stored in the priority set
     */
    std::size_t size() const {
        return data.size();
    }

private:
    bool has_hash(uint64_t hash) const {
        return indices.find(hash) != indices.end();
    }

    // Parent index from child
    std::size_t get_par(std::size_t idx) const {
        return (idx - 1) / 2;
    }

    // Left child index from parent
    std::size_t get_left(std::size_t idx) const {
        return idx * 2 + 1;
    }

    // Right child index from parent
    std::size_t get_right(std::size_t idx) const {
        return idx * 2 + 2;
    }

    void swap_elements(std::size_t idx1, std::size_t idx2) {
        std::swap(data[idx1], data[idx2]);
        std::swap(indices.at(hasher(data[idx1])), indices.at(hasher(data[idx2])));
    }

    void swim(std::size_t idx) {
        std::size_t par_idx = get_par(idx);
        while (idx > 0 && comper(data[idx], data[par_idx])) {
            swap_elements(idx, par_idx);
            idx = par_idx;
            par_idx = get_par(idx);
        }
    }

    void sink(std::size_t idx) {
        while (true) {
            std::size_t left = get_left(idx);
            std::size_t right = get_right(idx);
            std::size_t swap_idx = idx;

            // Check children
            if (left < size() && comper(data[left], data[swap_idx])) {
                swap_idx = left;
            }
            if (right < size() && comper(data[right], data[swap_idx])) {
                swap_idx = right;
            }

            // No swap, done fixing heap
            if (idx == swap_idx) {
                return;
            }

            swap_elements(idx, swap_idx);
            idx = swap_idx;
        }
    }

    CompareT comper;
    HashT hasher;
    std::vector<T> data;                                  // Data storage
    std::unordered_map<uint64_t, std::size_t> indices;    // Mapping of element to index in queue
};

}    // namespace tpl_search

#endif    // PRA_UTIL_PQ_H
