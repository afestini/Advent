export module day7;

import std;
import utils;

using namespace std;
using namespace std::chrono;


static uint64_t split(Map2D& map, int x, int y) {
	if (!map.is_in_bounds({ x, y })) return 0;

	char& cell = map[x, y];
	if (cell == '|') return 0;

	if (cell == '^')
		return 1 + split(map, x - 1, y) + split(map, x + 1, y);

	cell = '|';
	return split(map, x, y + 1);
}


export void day7_1() {
	const auto start_time = high_resolution_clock::now();

	Map2D map("day7.txt");

	const auto col = distance(map.grid.begin(), ranges::find(map.grid, 'S'));
	const auto splits = split(map, static_cast<int>(col), 1);

	const auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start_time);
	println("Day 7a: {} ({})", splits, duration);
}


// Same as before, except we don't abort if we hit an existing timeline,
// instead we fetch the cached result from earlier, or this will take forever
static uint64_t count_timelines(Map2D& map, int x, int y) {
	static unordered_map<int, uint64_t> cache;

	if (!map.is_in_bounds({ x, y })) return 0;

	char& cell = map[x, y];
	if (cell == '|') return cache[x];

	if (cell == '^')
		return 1 + count_timelines(map, x - 1, y) + count_timelines(map, x + 1, y);

	cell = '|';
	return cache[x] = count_timelines(map, x, y + 1);
}


export void day7_2() {
	const auto start_time = high_resolution_clock::now();

	Map2D map("day7.txt");

	const auto col = distance(map.grid.begin(), ranges::find(map.grid, 'S'));
	const auto timelines = 1 + count_timelines(map, static_cast<int>(col), 1);

	const auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start_time);
	println("Day 7b: {} ({})", timelines, duration);
}
