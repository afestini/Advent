export module day11;

import std;

using namespace std;


static vector<string> ReadMap() {
	ifstream input("day11.txt");
	if (!input) return {};

	vector<string> map(1);
	while (getline(input, map.back())) {
		map.emplace_back();
	}
	map.pop_back();

	return map;
}


struct Coord { int64_t x; int64_t y; };


static int64_t ExpandAndMeasure(int64_t expansion) {
	const auto map = ReadMap();

	vector<Coord> galaxies;

	// Fill galaxies and expand vertically on the fly
	int64_t empty_rows = 0;
	for (auto [r, line] : map | views::enumerate) {
		const auto count_before = galaxies.size();
		for (auto [c, ch] : line | views::enumerate) {
			if (ch=='#') galaxies.emplace_back(c, r + empty_rows);
		}
		if (galaxies.size() == count_before) empty_rows += (expansion - 1);
	}

	// Sort by x to make life a LOT easier
	ranges::sort(galaxies, {}, &Coord::x);

	// Expand horizontally
	int64_t last_gal_x = 0;
	int64_t empty_cols = 0;
	for (auto& g : galaxies) {
		empty_cols += max(0LL, (g.x - last_gal_x - 1)) * (expansion - 1);
		last_gal_x = g.x;
		g.x += empty_cols;
	}

	// Sum up all distances
	uint64_t sum = 0;
	for (auto [i, g1] : galaxies | views::enumerate) {
		for (const auto& g2 : span(galaxies).subspan(i + 1)) {
			sum += abs(g2.x - g1.x) + abs(g2.y - g1.y);
		}
	}

	return sum;
}


export void day11_1() {
	const auto start = chrono::high_resolution_clock::now();

	const auto sum = ExpandAndMeasure(2);

	const auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
	println("Day 11a: {} ({})", sum, duration);
}


export void day11_2() {
	const auto start = chrono::high_resolution_clock::now();

	const auto sum = ExpandAndMeasure(1'000'000);

	const auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
	println("Day 11b: {} ({})", sum, duration);
}
