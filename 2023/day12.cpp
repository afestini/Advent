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
	while (pattern.starts_with('.')) pattern.remove_prefix(1);

	if (groups.empty())
		return (ranges::contains(pattern, '#')) ? 0 : 1;

	if (pattern.empty()) return 0;

	auto [entry, added] = cache.try_emplace(pattern.size() << 32 | groups.size());
	if (!added)
		return entry->second;

	const int64_t sub1 = (pattern[0] == '?') ? Solve(pattern.substr(1), groups, cache) : 0;

	int64_t sub2 = 0;
	if (pattern.substr(0, groups[0]).contains('.')) sub2 = 0;
	else if (ssize(pattern) == groups[0] && groups.size() == 1) sub2 = 1;
	else if (ssize(pattern) > groups[0] && pattern[groups[0]] != '#')
		sub2 = Solve(pattern.substr(groups[0] + 1), groups.subspan(1), cache);

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

	auto gears = ReadMap();

	for_each(execution::par_unseq, gears.begin(), gears.end(), [](auto& g) {
		g.pattern = views::repeat(g.pattern, 5) | views::join_with('?') | ranges::to<string>();
		g.groups = views::repeat(g.groups, 5) | views::join | ranges::to<vector>();
	});

	const int64_t sum = transform_reduce(execution::par_unseq, gears.begin(), gears.end(), 0LL, plus<>(),
		[](const auto& g) {
			Cache cache;
			return Solve(g.pattern, g.groups, cache);
		});

	const auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
	println("Day 12b: {} ({})", sum, duration);
}
