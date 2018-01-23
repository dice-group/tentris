//
// Created by me on 17.01.18.
//

#ifndef LIBSPARSETENSOR_UNDIRECTEDGRAPH_HPP
#define LIBSPARSETENSOR_UNDIRECTEDGRAPH_HPP

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "../../util.hpp"

using std::unordered_set;
using std::unordered_map;
using std::vector;
using std::set;

template<typename T>
class UndirectedGraph {
public:
    UndirectedGraph() {}

private:
    unordered_set<T> nodes{};
    unordered_map<T, unordered_set<T >> edges{};

public:
    void addFullGraph(const set<T> &nodes) { // TODO: test
        auto node_a = nodes.begin();
        for (; node_a != nodes.end(); node_a++) {
            auto node_b = node_a;
            for (; node_b != nodes.end(); node_b++) {
                addEdge(*node_a, node_b);
            }
        }
    }

    void addEdge(T x, T y) {
        nodes.insert(x);
        unordered_set<T> &adjacent_nodes_x = edges[x];
        adjacent_nodes_x.insert(y);

        nodes.insert(y);
        unordered_set<T> &adjacent_nodes_y = edges[y];
        adjacent_nodes_y.insert(x);
    }

    unordered_set<unordered_set<T>> getConnectedComponents() {
        unordered_set<T> unfinished_nodes{this->nodes};

        unordered_set<T> open_nodes{};

        unordered_set<unordered_set<T>> connected_components{};

        do {
            unordered_set<T> connected_component{};

            T first_node = *unfinished_nodes.begin();
            open_nodes.insert(first_node);
            connected_component.insert(first_node);
            unfinished_nodes.erase(first_node);

            while (not open_nodes.empty()) {
                T node = *open_nodes.begin();
                open_nodes.erase(node);
                const unordered_set<T> &adjacent_nodes = edges[node];
                for (T adj_node : adjacent_nodes) {
                    if (unfinished_nodes.count(adj_node)) {
                        connected_component.insert(adj_node);
                        open_nodes.insert(adj_node);
                        unfinished_nodes.erase(adj_node);
                    }
                }
            }

            connected_components.emplace(connected_component);
        } while (not unfinished_nodes.empty());

        return connected_components;
    }


};


#endif //LIBSPARSETENSOR_UNDIRECTEDGRAPH_HPP
