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
		}
	}

	const auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start_time);
	println("Day 6a: {} ({})", pos_count, duration);
}


static char dir2bit(Vec2i dir) {
	static constexpr array<char, 6> bits{{0x0, 0x1, 0x2, 0x0, 0x4, 0x8}};
	return bits[2 * (dir.y + 1) + dir.x + 1];
}


static bool check(Map2D map, Map2D::Pos pos, Vec2i dir) {
	vector<pair<Map2D::Pos, uint8_t>> tmp_info;

	for (; map.is_in_bounds(pos); pos+=dir) {
		if (map[pos] != '#') continue;

		// Did we hit the obstacle from this direction before? Otherwise mark it (vector is a tad faster)
		auto it = ranges::find(tmp_info, pos, &pair<Map2D::Pos, uint8_t>::first);
		auto& info = it != tmp_info.end() ? it->second : tmp_info.emplace_back(pos, '\0').second;

		const auto dir_bit = dir2bit(dir);
		if (info & dir_bit) return true;
		info |= dir_bit;

		pos -= dir;
		dir = {-dir.y, dir.x};
	}
	return false;
}


export void day6_2() {
	const auto start_time = high_resolution_clock::now();

	Map2D map("day6.txt");

	auto pos = find_start(map);
	Vec2i dir {0,-1};

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
			const auto tmp = exchange(map[pos], '#');
			futures.emplace_back(async(launch::async, check, map, pos, dir));
			tile = tmp;
		}
	}

	const auto options = ranges::count(futures, true, &future<bool>::get);

	const auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start_time);
	println("Day 6b: {} ({})", options, duration);
}
