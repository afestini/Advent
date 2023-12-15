export module day8;

import std;

using namespace std;

struct Map {
	string directions;

	unordered_map<string, pair<string, string>> nodes;
};


static Map ParseMap() {
	ifstream input("day8.txt");
	if (!input) return {};

	Map the_map;

	getline(input, the_map.directions);

	string line;
	string key, ignore, left, right;
	while (getline(input, line)) {
		if (line.empty()) continue;
		ispanstream(line) >> key >> ignore >> left >> right;
		the_map.nodes.emplace(key, pair(left.substr(1, 3), right.substr(0, 3)));
	}

	return the_map;
}


template<typename Cond>
static uint64_t FollowInstructions(const Map& map, string pos, const Cond& cond) {
	uint64_t steps = 0;
	for (const auto& instr : views::repeat(map.directions)) {
		for (auto lr : instr) {
			pos = (lr == 'L') ? map.nodes.at(pos).first : map.nodes.at(pos).second;
			++steps;
			if (cond(pos)) return steps;
		}
	}
	return steps;
}



export void day8_1() {
	const auto start = chrono::high_resolution_clock::now();

	const auto map = ParseMap();

	const auto steps = FollowInstructions(map, "AAA", bind_front(equal_to(), "ZZZ"));

	const auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
	println("Day 8a: {} ({})", steps, duration);
}


export void day8_2() {
	const auto start = chrono::high_resolution_clock::now();

	const auto map = ParseMap();

	const auto ends_in = [](string_view s, char ch) { return s.ends_with(ch); };

	auto nodes = map.nodes | views::keys | views::filter(bind_back(ends_in, 'A')) | ranges::to<vector>();
	vector<uint64_t> steps(nodes.size());

	for_each(execution::par_unseq, nodes.begin(), nodes.end(), [&](const auto& node) {
		const auto i = &node - nodes.data();
		steps[i] = FollowInstructions(map, node, bind_back(ends_in, 'Z'));
	});

	const auto LCM = ranges::fold_left(steps, 1ULL, lcm<uint64_t, uint64_t>);

	const auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
	println("Day 8b: {} ({})", LCM, duration);
}
