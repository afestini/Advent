export module day5;

import std;

using namespace std;


struct RangeMap {
	string name; // Just for debugging
	struct Entry {
		uint64_t dst = 0;
		uint64_t src = 0;
		uint64_t sz = 0;
	};
	vector<Entry> entries;

	uint64_t LookUp(uint64_t x) const {
		for (const auto& e : entries) {
			if (x >= e.src && x < e.src + e.sz) {
				return e.dst + (x - e.src);
			}
		}
		return x;
	}
};

struct Almanac {
	vector<uint64_t> seeds;
	vector<RangeMap> tables;
};


Almanac ParseTables() {
	ifstream input("day5.txt");
	if (!input) return {};

	Almanac almanac;
	string line;
	uint64_t seed{};
	RangeMap* current_map = nullptr;
	while (getline(input, line)) {
		if (line.starts_with("seeds:")) {
			ispanstream str(string_view(line).substr(6));
			while (str >> seed) almanac.seeds.emplace_back(seed);
		}
		else if (line.ends_with("map:")) {
			current_map = &almanac.tables.emplace_back(line);
		}
		else if (!line.empty() && current_map) {
			auto& entry = current_map->entries.emplace_back();
			ispanstream(string_view(line)) >> entry.dst >> entry.src >> entry.sz;
		}
	}
	return almanac;
}


static uint64_t FindLocation(const Almanac& almanac, uint64_t seed) {
	uint64_t search = seed;
	for (const auto& table : almanac.tables) {
		search = table.LookUp(search);
	}
	return search;
}


export void day5_1() {
	const auto almanac = ParseTables();

	uint64_t min_loc = numeric_limits<uint64_t>::max();

	for (auto seed : almanac.seeds) {
		const auto loc = FindLocation(almanac, seed);
		min_loc = min(min_loc, loc);
	}
	println("Day 5a: {}", min_loc);
}


export void day5_2() {
	const auto almanac = ParseTables();

	uint64_t min_loc = numeric_limits<uint64_t>::max();
	mutex mtx;

	const auto start = chrono::high_resolution_clock::now();

	// Too lazy for a smarter approach... bring coffee
	const auto seed_ranges = almanac.seeds | views::chunk(2);
	for_each(execution::par_unseq, seed_ranges.begin(), seed_ranges.end(),
			 [&almanac, &mtx, &min_loc](const auto& range)
	{
		uint64_t range_min_loc = numeric_limits<uint64_t>::max();

		for (auto seed : views::iota(range.front(), range.front() + range.back())) {
			const auto loc = FindLocation(almanac, seed);
			range_min_loc = min(range_min_loc, loc);
		}

		lock_guard lk(mtx);
		min_loc = min(min_loc, range_min_loc);
	});

	println("Search: {:%T}", chrono::high_resolution_clock::now() - start);
	println("Day 5b: {}", min_loc);
}
