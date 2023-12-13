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
		const auto it = ranges::find_if(entries, [x](const auto& e) { return x >= e.src && x < e.src + e.sz; });
		return (it != entries.end()) ? (it->dst + (x - it->src)) : x;
	}

	uint64_t ReverseLookUp(uint64_t x) const {
		const auto it = ranges::find_if(entries, [x](const auto& e) { return x >= e.dst && x < e.dst + e.sz; });
		return (it != entries.end()) ? (it->src + (x - it->dst)) : x;
	}
};

struct Almanac {
	vector<uint64_t> seeds;
	vector<RangeMap> tables;
};


static Almanac ParseTables() {
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


static uint64_t FindSeed(const Almanac& almanac, uint64_t loc) {
	uint64_t search = loc;
	for (const auto& table : almanac.tables | views::reverse) {
		search = table.ReverseLookUp(search);
	}
	return search;
}


static bool HaveSeed(const Almanac& almanac, uint64_t seed) {
	const auto seed_ranges = almanac.seeds | views::chunk(2);
	for (const auto& range : seed_ranges) {
		if (seed >= range.front() && seed < range.front() + range.back()) return true;
	}
	return false;
}


export void day5_1() {
	const auto start = chrono::high_resolution_clock::now();

	const auto almanac = ParseTables();

	uint64_t min_loc = numeric_limits<uint64_t>::max();

	for (auto seed : almanac.seeds) {
		const auto loc = FindLocation(almanac, seed);
		min_loc = min(min_loc, loc);
	}
	
	const auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
	println("Day 5a: {} ({})", min_loc, duration);
}



export void day5_2() {
	const auto start = chrono::high_resolution_clock::now();

	const auto almanac = ParseTables();

	atomic<uint64_t> min_loc = numeric_limits<uint64_t>::max();
	mutex mtx;

	// Seed ranges are too big and would need to be searched entirely.
	// Instead, reverse search (location -> seed), so we can abort on the first hit
	// Search in steps of 20 million, processing chunks of 50k in parallel.
	// Abort once we are above the current min location
	// Brings it down from 45s (parallel search over seed ranges) to ~75ms

	auto search_from = 0;
	while (search_from < min_loc) {
		const auto ranges = views::iota(search_from, search_from + 20'000'000) | views::chunk(50'000);
		for_each(execution::par_unseq, ranges.begin(), ranges.end(), [&almanac, &min_loc, &mtx](const auto& range) {
			for (auto loc : range) {
				if (loc >= min_loc) return;

				const auto seed = FindSeed(almanac, loc);
				if (HaveSeed(almanac, seed)) {
					lock_guard lk(mtx);
					min_loc = min<uint64_t>(min_loc, loc);
					return;
				}
			}
		});
		search_from += 20'000'000;
	}

	const auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
	println("Day 5b: {} ({})", min_loc.load(), duration);
}
