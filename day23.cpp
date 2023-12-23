export module day23;

import std;

using namespace std;


static vector<string> ReadInput() {
	ifstream input("day23.txt");

	vector<string> data(1);
	while (getline(input, data.back())) {
		data.emplace_back();
	}
	data.pop_back();
	return data;
}


namespace {
	struct Vec {
		int x;
		int y;

		auto operator<=>(const Vec&) const = default;
		Vec operator+(Vec b) const { return {x + b.x, y + b.y}; }
		Vec operator-() const { return {-x, -y}; }
	};
}

static const array<Vec, 4> dirs = {{{-1, 0}, {1, 0}, {0, -1}, {0, 1}}};


struct Node {
	Vec pos;
	struct EdgeInfo { Vec dir; int steps; };
	mutable unordered_map<const Node*, EdgeInfo> edges;
};
using NodeSet = vector<Node>;


static auto GetNodes(span<const string> input) {
	NodeSet nodes;

	for (int y = 1; y < ssize(input) - 1; ++y) {
		for (int x = 1; x < ssize(input[0]) - 1; ++x) {
			if (input[y][x] != '.') continue;

			const auto outs = ranges::count_if(dirs, [&input, x, y](auto dir) {
				return input[y + dir.y][x + dir.x] != '#';
			});
			if (outs > 2) nodes.emplace_back(Vec{x, y});
		}
	}
	return nodes;
}


static void FollowPath(span<const string> input, const NodeSet& nodes, const Node& from, Vec start_dir, bool slopes_block) {
	static unordered_map<char, Vec> slopes = {{'>', {1, 0}}, {'<', {-1, 0}}, {'^', {0, -1}}, {'v', {0, 1}}};

	Vec pos = from.pos;
	Vec dir = start_dir;
	int steps = 0;

	for (;;) {
		pos = pos + dir;
		++steps;

		const auto tile = input[pos.y][pos.x];
		if (tile == '#') return;

		if (slopes_block && slopes[tile] == -dir) return;

		if (auto it = ranges::find(nodes, pos, &Node::pos); it != nodes.end()) {
			from.edges.emplace(to_address(it), Node::EdgeInfo{start_dir, steps});

			if (!slopes_block) {
				it->edges.emplace(&from, Node::EdgeInfo{-dir, steps});
			}
			return;
		}

		for (auto test_dir : dirs) {
			const Vec test = pos + test_dir;
			if (test_dir != -dir && input[test.y][test.x] != '#') {
				dir = test_dir;
				break;
			}
		}
	}
}


static void ConnectEdges(span<const string> input, const NodeSet& nodes, bool slopes_block) {
	for (auto& node : nodes) {
		for (auto dir : dirs) {
			const auto y = node.pos.y + dir.y;
			if (y < 0 || y >= ssize(input)) continue;
			if (ranges::contains(node.edges | views::values, dir, &Node::EdgeInfo::dir)) continue;

			FollowPath(input, nodes, node, dir, slopes_block);
		}
	}
}


static int SearchDF(const Node& node, const Node& goal, int steps, vector<int8_t>& visited, size_t stride) {
	if (&node == &goal) return steps;

	visited[node.pos.y * stride + node.pos.x] = true;

	int max_steps = 0;
	for (const auto& [to, edge_info] : node.edges) {
		if (!visited[to->pos.y * stride + to->pos.x]) {
			max_steps = max(max_steps, SearchDF(*to, goal, steps + edge_info.steps, visited, stride));
		}
	}

	visited[node.pos.y * stride + node.pos.x] = false;
	return max_steps;
}


static int FindLongestPath(span<const string> input, bool slopes_block) {
	auto nodes = GetNodes(input);
	const auto& start = nodes.emplace_back(Vec{static_cast<int>(input[0].find('.')), 0});
	const auto& goal = nodes.emplace_back(Vec{static_cast<int>(input.back().find('.')), static_cast<int>(ssize(input)) - 1});

	ConnectEdges(input, nodes, slopes_block);

	vector<int8_t> visited(input.size() * input[0].size());
	return SearchDF(start, goal, 0, visited, input[0].size());
}


export void day23_1() {
	const auto start_time = chrono::high_resolution_clock::now();

	const auto input = ReadInput();
	const auto max_steps = FindLongestPath(input, true);

	const auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start_time);
	println("Day 23a: {} ({})", max_steps, duration);
}


export void day23_2() {
	const auto start_time = chrono::high_resolution_clock::now();

	const auto input = ReadInput();
	const auto max_steps = FindLongestPath(input, false);

	const auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start_time);
	println("Day 23b: {} ({})", max_steps, duration);
}
