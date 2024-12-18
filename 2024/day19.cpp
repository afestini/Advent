export module day19;

import std;

using namespace std;
using namespace std::chrono;


struct Towels {
	vector<string> types;
	vector<string> designs;
};


static auto file_input() {
	ifstream input("day19.txt");

	Towels towels;
	string line;

	getline(input, line);
	for (auto type : views::split(line, ", "sv)) {
		towels.types.emplace_back(type.begin(), type.end());
	}

	getline(input, line);

	while (getline(input, towels.designs.emplace_back()));

	towels.designs.pop_back();

	return towels;
}


static bool check_pattern(const vector<string>& types, string_view pattern) {
	if (pattern.empty()) return true;

	for (const auto& type : types) {
		if (pattern.starts_with(type) && check_pattern(types, pattern.substr(type.length())))
			return true;
	}
	return false;
}


export void day19_1() {
	const auto start_time = high_resolution_clock::now();

	auto towels = file_input();

	const auto possible = ranges::count_if(towels.designs, [&towels](const auto& pattern) {
		return check_pattern(towels.types, pattern);
	});

	const auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start_time);
	println("Day 19a: {} ({})", possible, duration);
}


static unordered_map<string_view, uint64_t> design_cache;

static uint64_t count_pattern_combinations(const vector<string>& types, string_view pattern) {
	if (pattern.empty()) return 1;

	auto [entry, is_new] = design_cache.try_emplace(pattern);
	if (!is_new) return entry->second;

	uint64_t ways = 0;
	for (const auto& type : types) {
		if (pattern.starts_with(type))
			ways += count_pattern_combinations(types, pattern.substr(type.length()));
	}

	return entry->second = ways;
}


export void day19_2() {
	const auto start_time = high_resolution_clock::now();

	auto towels = file_input();

	const auto combinations = towels.designs | views::transform(bind_front(count_pattern_combinations, towels.types));
	const auto total = reduce(combinations.begin(), combinations.end());

	const auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start_time);
	println("Day 19b: {} ({})", total, duration);
}
