#ifndef SPARSETENSOR_EINSUM_UTIL_UNDIRECTEDGRAPH_HPP
#define SPARSETENSOR_EINSUM_UTIL_UNDIRECTEDGRAPH_HPP

#include "../../util/All.hpp"
#include <unordered_map>
#include <unordered_set>

using std::unordered_set;
using std::unordered_map;
using std::set;


namespace sparsetensor::operations::util {

/**
 * Simple undirected Graph implementation.
 * @tparam T type of the nodes.
 */
    template<typename T>
    class UndirectedGraph {
    public:
        /**
         * New empty UndirectedGraph.
         */
        UndirectedGraph() = default;

    private:
        /**
         * Set of all nodes.
         */
        unordered_set<T> nodes{};
        /**
         * Set of all directed edges. (a,a)-edged are allowed.
         */
        unordered_map<T, unordered_set<T >> edges{};

    public:
        /**
         * Adds all edges between the given nodes (complete graph).
         * @param nodes nodes to span the complete graph.
         */
        void addCompleteGraph(const set<T> &nodes) {
            // add all combinations
            auto node_a = nodes.begin();
            for (; node_a != nodes.end(); node_a++) {
                auto node_b = node_a;
                for (; node_b != nodes.end(); node_b++) {
                    addEdge(*node_a, *node_b);
                }
            }
        }

        /**
         * Add a undirected edge between two nodes.
         * @param node_a first node
         * @param node_b second node
         */
        void addEdge(T node_a, T node_b) {
            // insert edge from a to b
            nodes.insert(node_a);
            unordered_set<T> &adjacent_nodes_x = edges[node_a];
            adjacent_nodes_x.insert(node_b);

            // insert edge from b to a
            nodes.insert(node_b);
            unordered_set<T> &adjacent_nodes_y = edges[node_b];
            adjacent_nodes_y.insert(node_a);
        }

        /**
         * Get all connected components e.g. all sets of nodes that are reachable form each other.
         * @return set of connected components.
         */
        unordered_set<unordered_set<T>> getConnectedComponents() const {
            // this is basically a breadth first search
            // see: https://en.wikipedia.org/wiki/Breadth-first_search
            unordered_set<T> unfinished_nodes{this->nodes};

            unordered_set<T> open_nodes{};

            unordered_set<unordered_set<T>> connected_components{};

            while (not unfinished_nodes.empty()) {
                unordered_set<T> connected_component{};

                T first_node = *unfinished_nodes.begin();
                open_nodes.insert(first_node);
                connected_component.insert(first_node);
                unfinished_nodes.erase(first_node);

                while (not open_nodes.empty()) {
                    T node = *open_nodes.begin();
                    open_nodes.erase(node);
                    const unordered_set<T> &adjacent_nodes = edges.at(node);
                    for (T adj_node : adjacent_nodes) {
                        if (unfinished_nodes.count(adj_node)) {
                            connected_component.insert(adj_node);
                            open_nodes.insert(adj_node);
                            unfinished_nodes.erase(adj_node);
                        }
                    }
                }

                connected_components.emplace(connected_component);
            }

            return connected_components;
        }

        /**
         * Get all neignbors of a node.
         * @param node the node.
         * @return the neigbors.
         */
        unordered_set<T> neighbors(const T node) const {
            return edges.at(node);
        }

        /**
         * Get all nodes.
         * @return all nodes.
         */
        unordered_set<T> getNodes() const {
            return nodes;
        }

        /**
         * Resets the graph to an empty graph.
         */
        void clear() {
            nodes.clear();
            edges.clear();
        }


        /**
         * Iterator of the nodes.
         * @return Node iterator
         */
        typename unordered_set<T>::const_iterator cbegin() const {
            return nodes.begin();
        };

        /**
         * End of nodes Iterator.
         * @return iterator end
         */
        typename unordered_set<T>::const_iterator cend() const {
            return nodes.end();
        };
    };

}

#endif //SPARSETENSOR_EINSUM_UTIL_UNDIRECTEDGRAPH_HPP
