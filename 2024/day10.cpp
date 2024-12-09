export module day10;

import std;
import utils;

using namespace std;
using namespace std::chrono;


static size_t evaluate_trail(const Map2D& map, Map2D::Pos start) {
	static constexpr array<Map2D::Pos, 4> directions {{{-1,0}, {0,-1}, {0,1}, {1,0}}};

	queue<Map2D::Pos> heads;
	heads.emplace(start);

	set<Map2D::Pos> peaks;
	while (!heads.empty()) {
		const auto pos = heads.front();
		const auto height = map[pos];
		heads.pop();

		if (height == '9') {
			peaks.emplace(pos);
			continue;
		}

		for (auto dir : directions) {
			const auto try_dir = pos + dir;
			if (map.is_in_bounds(try_dir) && map[try_dir] == height + 1)
				heads.emplace(try_dir);
		}
	}
	return peaks.size();
}

export void day10_1() {
	const auto start_time = high_resolution_clock::now();

	Map2D map("day10.txt");

	size_t total = 0;

	for (int y = 0; y < map.Height(); ++y) {
		for (int x = 0; x < map.Width(); ++x) {
			if (map[Map2D::Pos(x, y)] == '0')
				total += evaluate_trail(map, Map2D::Pos(x, y));
		}
	}

	const auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start_time);
	println("Day 10a: {} ({})", total, duration);
}


static size_t rate_trail(const Map2D& map, Map2D::Pos start) {
	static constexpr array<Map2D::Pos, 4> directions {{{-1,0}, {0,-1}, {0,1}, {1,0}}};

	queue<Map2D::Pos> heads;
	heads.emplace(start);

	size_t rating = 0;
	while (!heads.empty()) {
		const auto pos = heads.front();
		const auto height = map[pos];
		heads.pop();

		if (height == '9') {
			++rating;
			continue;
		}

		for (auto dir : directions) {
			const auto try_dir = pos + dir;
			if (map.is_in_bounds(try_dir) && map[try_dir] == height + 1)
				heads.emplace(try_dir);
		}
	}
	return rating;
}


export void day10_2() {
	const auto start_time = high_resolution_clock::now();

	Map2D map("day10.txt");

	size_t total = 0;

	for (int y = 0; y < map.Height(); ++y) {
		for (int x = 0; x < map.Width(); ++x) {
			if (map[Map2D::Pos(x, y)] == '0')
				total += rate_trail(map, Map2D::Pos(x, y));
		}
	}

	const auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start_time);
	println("Day 10b: {} ({})", total, duration);
}
