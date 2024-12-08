export module day6;

import std;
import utils;

using namespace std;
using namespace std::chrono;


static Map2D::Pos find_start(const Map2D& map) {
	Map2D::Pos pos;
	for (pos.y = 0; pos.y < map.Height(); ++pos.y) {
		for (pos.x = 0; pos.x < map.Width(); ++pos.x) {
			if (map[pos] == '^') return pos;
		}
	}
	return pos;
}


export void day6_1() {
	const auto start_time = high_resolution_clock::now();

	Map2D map("day6.txt");

	auto pos = find_start(map);
	Vec2i dir {0,-1};

	int pos_count = 1; // Count start position to avoid extra checks later
	for (;;) { // Bravely assume start position is on the map
		auto& tile = map[pos];
		if (tile == '.') {
			++pos_count;
			tile = 'X';
		}

		pos += dir;
		if (!map.is_in_bounds(pos)) break;

		if (map[pos] == '#') { // rotate right.. gotta love simple 2D
			pos -= dir; // This is the rare case, so undoing is cheaper
			dir = {-dir.y, dir.x};
			pos += dir;
		}
	}

	const auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start_time);
	println("Day 6a: {} ({})", pos_count, duration);
}


static char dir2bit(Vec2i dir) {
	if (dir.y) return dir.y < 0 ? 0x1 : 0x2;
	return dir.x < 0 ? 0x4 : 0x8;
};


static bool check(Map2D map, Map2D::Pos pos, Vec2i dir) {
	std::map<Map2D::Pos, char> tmp_info;

	for (;;) {
		auto next_pos = pos + dir;
		if (!map.is_in_bounds(next_pos)) return false;

		if (map[next_pos] == '#') {
			const auto dir_bit = dir2bit(dir);
			auto& info = tmp_info[next_pos];
			if (info & dir_bit) return true;
			info |= dir_bit;

			dir = {-dir.y, dir.x};
			next_pos = pos + dir;
			if (!map.is_in_bounds(next_pos)) return false;
		}

		pos = next_pos;
	}
}


export void day6_2() {
	const auto start_time = high_resolution_clock::now();

	Map2D map("day6.txt");

	auto start_pos = find_start(map);
	auto pos = start_pos;
	Vec2i dir {0,-1};

	vector<future<bool>> futures;

	for (;;) {
		map[pos] = 'X';

		auto next_pos = pos + dir;
		if (!map.is_in_bounds(next_pos)) break;

		while (map[next_pos] == '#') {
			dir = {-dir.y, dir.x};
			next_pos = pos + dir;
			if (!map.is_in_bounds(next_pos)) break;
		}

		if (auto& tile = map[next_pos]; tile == '.') {
			auto tmp = exchange(tile, '#');
			futures.emplace_back(async(launch::async, check, map, start_pos, Vec2(0, -1)));
			tile = tmp;
		}

		pos = next_pos;
	}

	const auto options = ranges::count(futures, true, &future<bool>::get);

	const auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start_time);
	println("Day 6b: {} ({})", options, duration);
}
