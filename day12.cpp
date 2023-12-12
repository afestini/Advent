export module day12;

import std;

using namespace std;


struct Line {
	string pattern;
	vector<int64_t> groups;
};

using Cache = unordered_map<uint64_t, int64_t>;


static vector<Line> ReadMap() {
	ifstream input("day12.txt");

	vector<Line> gears;

	string line;
	while (getline(input, line)) {
		auto& entry = gears.emplace_back();
		ispanstream str(line);
		str >> entry.pattern;
		while (str) {
			int group;
			(str >> group).ignore(1);
			entry.groups.emplace_back(group);
		}
	}
	return gears;
}


static int64_t Solve(string_view pattern, span<const int64_t> groups, Cache& cache) {
	if (groups.empty())
		return (ranges::count(pattern, '#') == 0) ? 1 : 0;

	if (pattern.empty())
		return groups.empty() ? 1 : 0;

	auto [entry, added] = cache.try_emplace(pattern.size() << 32 | groups.size());
	if (!added)
		return entry->second;

	int64_t sub1 = 0;
	if (".?"s.contains(pattern[0])) {
		sub1 = Solve(pattern.substr(1), groups, cache);
	}

	int64_t sub2 = 0;
	if ("#?"s.contains(pattern[0])) {
		if (ssize(pattern) < groups[0]) sub2 = 0;
		else if (pattern.substr(0, groups[0]).contains('.')) sub2 = 0;
		else if (ssize(pattern) == groups[0]) sub2 = (groups.size() == 1) ? 1 : 0;
		else if (pattern[groups[0]] == '#') sub2 = 0;
		else sub2 = Solve(pattern.substr(groups[0] + 1), groups.subspan(1), cache);
	}

	return entry->second = sub1 + sub2;
}


export void day12_1() {
	const auto start = chrono::high_resolution_clock::now();

	int64_t sum = 0;
	auto gears = ReadMap();

	for (auto& g : gears) {
		Cache cache;
		sum += Solve(g.pattern, g.groups, cache);
	}

	const auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
	println("Day 12a: {} ({})", sum, duration);
}


export void day12_2() {
	const auto start = chrono::high_resolution_clock::now();

	int64_t sum = 0;
	auto gears = ReadMap();

	for (auto& g : gears) {
		g.pattern = views::repeat(g.pattern, 5) | views::join_with('?') | ranges::to<string>();
		g.groups = views::repeat(g.groups, 5) | views::join | ranges::to<vector>();
	}

	for (const auto& g : gears) {
		Cache cache;
		sum += Solve(g.pattern, g.groups, cache);
	}

	const auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
	println("Day 12b: {} ({})", sum, duration);
}
