export module day6;

import std;
import utils;

using namespace std;


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
	const auto start = chrono::high_resolution_clock::now();

	Map2D map("day6.txt");

	auto pos = find_start(map);
	Vec2 dir {0,-1};

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

	const auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
	println("Day 6a: {} ({})", pos_count, duration);
}


static char dir2bit(Vec2 dir) {
	if (dir.y) return dir.y < 0 ? 0x1 : 0x2;
	return dir.x < 0 ? 0x4 : 0x8;
};


static bool check(Map2D map, Map2D::Pos pos, Vec2 dir) {
	std::map<Map2D::Pos, char> tmp_info;
	auto dir_bit = dir2bit(dir);

	for (;;) {
		auto& info = tmp_info[pos];
		if (info & dir_bit) return true;

		info |= dir_bit;

		pos += dir;
		if (!map.is_in_bounds(pos)) return false;

		if (map[pos] == '#') {
			pos -= dir;
			dir = {-dir.y, dir.x};
			dir_bit = dir2bit(dir);
			pos += dir;
		}
	}
}


export void day6_2() {
	const auto start = chrono::high_resolution_clock::now();

	Map2D map("day6.txt");

	auto start_pos = find_start(map);
	auto pos = start_pos;
	Vec2 dir {0,-1};

	vector<future<bool>> futures;

	for (;;) {
		map[pos] = 'X';

		pos += dir;
		if (!map.is_in_bounds(pos)) break;

		if (auto& tile = map[pos]; tile == '#') {
			pos -= dir;
			dir = {-dir.y, dir.x};
		}
		else if (tile == '.') {
			auto tmp = exchange(map[pos], '#');
			futures.emplace_back(async(launch::async, check, map, start_pos, Vec2(0, -1)));
			tile = tmp;
		}
	}

	const auto options = ranges::count(futures, true, &future<bool>::get);

	const auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
	println("Day 6b: {} ({})", options, duration);
}
