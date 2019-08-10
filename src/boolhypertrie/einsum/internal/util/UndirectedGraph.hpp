#ifndef SPARSETENSOR_EINSUM_UTIL_UNDIRECTEDGRAPH_HPP
#define SPARSETENSOR_EINSUM_UTIL_UNDIRECTEDGRAPH_HPP

#include <set>
#include <map>


namespace einsum::internal::util {

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
		std::set<T> nodes{};
		/**
		 * Set of all directed edges. (a,a)-edged are allowed.
		 */
		std::map<T, std::set<T >> edges{};

	public:
		/**
		 * Adds all edges between the given nodes (complete graph).
		 * @param nodes nodes to span the complete graph.
		 */
		template<typename C>
		void addCompleteGraph(const C &new_nodes) {
			// add all combinations

			std::set<T> new_nodes_set{new_nodes.begin(), new_nodes.end()};

			for (const T &node_a : new_nodes_set)
				for (const T &node_b : new_nodes_set)
					addEdge(node_a, node_b);
		}

		/**
		 * Add a undirected edge between two nodes.
		 * @param node_a first node
		 * @param node_b second node
		 */
		void addEdge(T node_a, T node_b) {
			// insert edge from a to b
			nodes.insert(node_a);
			std::set<T> &adjacent_nodes_x = edges[node_a];
			adjacent_nodes_x.insert(node_b);

			// insert edge from b to a
			nodes.insert(node_b);
			std::set<T> &adjacent_nodes_y = edges[node_b];
			adjacent_nodes_y.insert(node_a);
		}

		/**
		 * Get all connected components e.g. all sets of nodes that are reachable form each other.
		 * @return set of connected components.
		 */
		[[nodiscard]] std::vector<std::set<T>> getConnectedComponents() const {
			// this is basically a breadth first search
			std::set<T> unfinished_nodes{this->nodes};

			std::set<T> open_nodes{};

			std::vector<std::set<T>> connected_components{};

			while (not unfinished_nodes.empty()) {
				std::set<T> connected_component{};

				T first_node = *unfinished_nodes.begin();
				open_nodes.insert(first_node);
				connected_component.insert(first_node);
				unfinished_nodes.erase(first_node);

				while (not open_nodes.empty()) {
					T node = *open_nodes.begin();
					open_nodes.erase(node);
					const std::set<T> &adjacent_nodes = edges.at(node);
					for (T adj_node : adjacent_nodes) {
						if (unfinished_nodes.count(adj_node)) {
							connected_component.insert(adj_node);
							open_nodes.insert(adj_node);
							unfinished_nodes.erase(adj_node);
						}
					}
				}

				connected_components.emplace_back(connected_component);
			}

			return connected_components;
		}

		/**
		 * Get all neignbors of a node.
		 * @param node the node.
		 * @return the neigbors.
		 */
		std::set<T> neighbors(const T node) const {
			return edges.at(node);
		}

		/**
		 * Get all nodes.
		 * @return all nodes.
		 */
		std::set<T> getNodes() const {
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
		typename std::set<T>::const_iterator cbegin() const {
			return nodes.cbegin();
		};

		/**
		 * End of nodes Iterator.
		 * @return iterator end
		 */
		typename std::set<T>::const_iterator cend() const {
			return nodes.cend();
		};
	};

}

#endif //SPARSETENSOR_EINSUM_UTIL_UNDIRECTEDGRAPH_HPP
