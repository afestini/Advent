export module day25;

import std;

using namespace std;


struct Graph {
	struct Node;

	struct Edge {
		Node* a;
		Node* b;
		auto operator<=>(const Edge& e) const = default;
	};

	struct Node {
		explicit Node(string_view id) : id(id) { names.emplace(id); }
		string id;
		set<string> names;
		set<Edge*> connections;
	};

	Graph Clone() const {
		Graph graph;
		for (const auto& edge : edges) {
			auto& node_a = graph.nodes.try_emplace(edge.a->id, edge.a->id).first->second;
			auto& node_b = graph.nodes.try_emplace(edge.b->id, edge.b->id).first->second;
			auto& new_edge = graph.edges.emplace_back(&node_a, &node_b);
			node_a.connections.emplace(&new_edge);
			node_b.connections.emplace(&new_edge);
		}
		return graph;
	}

	list<Edge> edges;
	unordered_map<string, Node> nodes;
};


static Graph ReadInput() {
	ifstream input("day25.txt");

	Graph graph;
	string line;
	string name;
	string con_name;
	while (getline(input, line)) {
		ispanstream str(span(line.begin(), line.end()));
		str >> name;
		name.pop_back();

		auto node = &graph.nodes.try_emplace(name, name).first->second;

		while (str >> con_name) {
			auto other_node = &graph.nodes.try_emplace(con_name, con_name).first->second;

			auto it = ranges::find(graph.edges, Graph::Edge(node, other_node));

			auto edge = (it != graph.edges.end()) ? to_address(it) : &graph.edges.emplace_back(node, other_node);

			node->connections.emplace(edge);
			other_node->connections.emplace(edge);
		}
	}
	return graph;
}


static size_t Random(size_t max) {
	static random_device rd;
	static mt19937 gen(rd());
	return uniform_int_distribution<size_t>(0, max)(gen);
}


static size_t Karger(const Graph& org_graph) {
	Graph graph = org_graph.Clone();

	while (graph.nodes.size() > 2) {
		const auto i = Random(graph.edges.size() - 1);
		auto edge_it = next(graph.edges.begin(), i);
		auto* a = edge_it->a;
		auto* b = edge_it->b;

		a->names.insert_range(b->names);

		for (auto con : b->connections) {
			if (con->a == b) con->a = a;
			if (con->b == b) con->b = a;

			if (con->a == a && con->b == a) {
				a->connections.erase(con);
				erase(graph.edges, *con);
			}
			else
				a->connections.emplace(con);
		}

		graph.nodes.erase(b->id);
	}

	if (graph.nodes.begin()->second.connections.size() == 3) {
		return ranges::fold_left(graph.nodes | views::values, 1, [](auto p, auto& n) {return p * n.names.size();});
	}
	return 0;
}


export void day25() {
	const auto start_time = chrono::high_resolution_clock::now();

	const auto graph = ReadInput();

	size_t result = 0;
	while (!result) result = Karger(graph);

	const auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start_time);
	println("Day 25: {} ({})", result, duration);
}
