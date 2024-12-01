export module day5;

import std;

using namespace std;


struct RangeMap {
	struct Entry {
		int64_t beg = 0;
		int64_t end = 0;
		int64_t delta = 0;
	};
	vector<Entry> entries;
	int64_t min_beg = numeric_limits<uint32_t>::max();
	int64_t max_end = 0;

	uint64_t LookUp(int64_t x) const {
		const auto it = ranges::find_if(entries, [x](const auto& e) { return x >= e.beg && x < e.end; });
		return x += (it == entries.end()) ? 0 : it->delta;
	}
};

struct Almanac {
	vector<int64_t> seeds;
	vector<RangeMap> tables;
};


static Almanac ParseTables() {
	ifstream input("day5.txt");

	Almanac almanac;

	string line;
	while (getline(input, line)) {
		if (line.starts_with("seeds:")) {
			ispanstream str(string_view(line).substr(6));
			int64_t seed{};
			while (str >> seed) almanac.seeds.emplace_back(seed);
		}
		else if (line.ends_with("map:")) {
			almanac.tables.emplace_back();
		}
		else if (!line.empty() && !almanac.tables.empty()) {
			int64_t dst, src, sz;
			ispanstream(string_view(line)) >> dst >> src >> sz;
			almanac.tables.back().entries.emplace_back(src, src + sz, dst - src);
			almanac.tables.back().min_beg = min(almanac.tables.back().min_beg, src);
			almanac.tables.back().max_end = max(almanac.tables.back().max_end, src + sz);
		}
	}
	return almanac;
}


static uint64_t FindLocation(const Almanac& almanac, int64_t seed) {
	return ranges::fold_left(almanac.tables, seed, [](int64_t seed, const RangeMap& map) {return map.LookUp(seed);});
}


export void day5_1() {
	const auto start = chrono::high_resolution_clock::now();

	const auto almanac = ParseTables();
	const uint64_t min_loc = ranges::min(almanac.seeds | views::transform(bind_front(FindLocation, almanac)));
	
	const auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
	println("Day 5a: {} ({})", min_loc, duration);
}


void SplitMap(RangeMap& from, const RangeMap& to) {
	// Use index, iterators get invalidated when adding entries
	for (size_t i = 0; i < from.entries.size(); ++i) {
		auto& src = from.entries[i];
		for (auto& dst : to.entries) {
			const auto split = dst.end - src.delta;
			if (split > src.beg && split < src.end) {
				from.entries.emplace_back(split, src.end, src.delta);
				src.end = split;
			}
		}
	}
	ranges::sort(from.entries, {}, &RangeMap::Entry::beg);
}


export void day5_2() {
	const auto start = chrono::high_resolution_clock::now();

	auto almanac = ParseTables();

	RangeMap seed_ranges;
	for (size_t i = 0; i < almanac.seeds.size(); i += 2) {
		seed_ranges.entries.emplace_back(almanac.seeds[i], almanac.seeds[i] + almanac.seeds[i + 1], 0);
	}

	// Fill possible gaps (0 - x; y - max)
	for (auto& table : almanac.tables) {
		if (table.min_beg > 0)
			table.entries.emplace_back(0, table.min_beg, 0);

		if (table.max_end < numeric_limits<uint32_t>::max())
			table.entries.emplace_back(table.max_end, numeric_limits<uint32_t>::max(), 0);

		ranges::sort(table.entries, {}, &RangeMap::Entry::beg);
	}

	for (const auto& in_out : almanac.tables | views::reverse | views::slide(2))
		SplitMap(in_out.back(), in_out.front());

	SplitMap(seed_ranges, almanac.tables[0]);

	const uint64_t min_loc = ranges::min(seed_ranges.entries | views::transform(
						[&almanac](const auto& r){return FindLocation(almanac, r.beg);}));

	const auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
	println("Day 5b: {} ({})", min_loc, duration);
}
