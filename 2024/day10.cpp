export module day10;

import std;
import utils;

using namespace std;
using namespace std::chrono;


static constexpr array<Map2D::Pos, 4> directions {{{-1,0}, {0,-1}, {0,1}, {1,0}}};


// Faster than a set for the number of peaks we'll get
vector<Map2D::Pos> peaks;

static void evaluate_trail(const Map2D& map, Map2D::Pos pos) {
	const auto height = map[pos];

	if (height == '9') {
		if (!ranges::contains(peaks, pos)) peaks.emplace_back(pos);
		return;
	}

	for (auto dir : directions) {
		const auto try_dir = pos + dir;
		if (map.is_in_bounds(try_dir) && map[try_dir] == height + 1)
			evaluate_trail(map, try_dir);
	}
}

export void day10_1() {
	const auto start_time = high_resolution_clock::now();

	Map2D map("day10.txt");

	size_t total = 0;

	for (Map2D::Pos pos {0,0}; pos.y < map.Height(); ++pos.y) {
		for (pos.x = 0; pos.x < map.Width(); ++pos.x) {
			if (map[pos] == '0') {
				peaks.clear();
				evaluate_trail(map, pos);
				total += peaks.size();
			}
		}
	}

	const auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start_time);
	println("Day 10a: {} ({})", total, duration);
}


static int rate_trail(const Map2D& map, Map2D::Pos pos) {
	const auto height = map[pos];

	if (height == '9') {
		return 1;
	}

	int count = 0;
	for (auto dir : directions) {
		const auto try_dir = pos + dir;
		if (map.is_in_bounds(try_dir) && map[try_dir] == height + 1)
			count += rate_trail(map, try_dir);
	}
	return count;
}


export void day10_2() {
	const auto start_time = high_resolution_clock::now();

	Map2D map("day10.txt");

	size_t total = 0;

	for (Map2D::Pos pos{0,0}; pos.y < map.Height(); ++pos.y) {
		for (pos.x = 0; pos.x < map.Width(); ++pos.x) {
			if (map[pos] == '0') {
				total += rate_trail(map, pos);
			}
		}
	}

	const auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start_time);
	println("Day 10b: {} ({})", total, duration);
}
