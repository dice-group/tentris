//
// Created by me on 17.01.18.
//

#ifndef LIBSPARSETENSOR_UNDIRECTEDGRAPH_HPP
#define LIBSPARSETENSOR_UNDIRECTEDGRAPH_HPP

#include <unordered_map>
#include <unordered_set>

class UndirectedGraph {
public:
    UndirectedGraph() {}

private:
    std::unordered_set<uint8_t> nodes{};
    std::unordered_map<uint8_t, std::unordered_set<uint8_t >> edges{};

    void addEdge(uint8_t x, uint8_t y) {
        nodes.insert(x);
        std::unordered_set<uint8_t> &adjacent_nodes = edges[x];
        adjacent_nodes.insert(y);
    }

    std::unordered_set<std::unordered_set<uint8_t>> getConnectedComponents() {
        std::unordered_set<uint8_t> unfinished_nodes{this->nodes};

        std::unordered_set<uint8_t> open_nodes{};

        std::unordered_set<std::unordered_set<uint8_t>> connected_components{};

        while (not unfinished_nodes.empty()) {
            std::unordered_set<uint8_t> connected_component{};

            uint8_t first_node = *unfinished_nodes.begin();
            open_nodes.insert(first_node);
            connected_component.insert(first_node);
            unfinished_nodes.erase(first_node);

            for (uint8_t node = *open_nodes.begin(); not open_nodes.empty(); node = *open_nodes.begin()) {
                open_nodes.erase(node);
                const std::unordered_set<uint8_t> &adjacent_nodes = edges[node];
                for (uint8_t adj_node : adjacent_nodes) {
                    if (unfinished_nodes.count(adj_node)) {
                        connected_component.insert(adj_node);
                        open_nodes.insert(adj_node);
                        unfinished_nodes.erase(adj_node);
                    }
                }
            }

            connected_components.emplace(connected_component);
        }
        while (not unfinished_nodes.empty());
        return connected_components;
    }


};


#endif //LIBSPARSETENSOR_UNDIRECTEDGRAPH_HPP
