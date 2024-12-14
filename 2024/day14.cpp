export module day14;

import std;
import utils;
import <Windows.h>;

using namespace std;
using namespace std::chrono;


struct Guard {
	Vec2i pos;
	Vec2i vel;
};

static const Vec2i map_size {101,103};
static const Vec2i half_size = map_size / 2;


static vector<Guard> file_input() {
	ifstream input("day14.txt");

	vector<Guard> guards;

	string line_str;
	while (getline(input, line_str)) {
		auto& guard = guards.emplace_back();

		string_view line(line_str);
		for (auto [v, token] : views::split(line, ' ') | views::enumerate) {
			auto& vec = (v == 0) ? guard.pos : guard.vel;

			const string_view coords = string_view(token).substr(2);
			for (auto [c, coord] : views::split(coords, ',') | views::enumerate) {
				from_chars(coord.data(), to_address(coord.end()), c == 0 ? vec.x : vec.y);
			}
		}

		// Fix negative directions here instead of dealing with it every time in part 2
		// Expecting vel to be less than the map size, but loop it just in case
		while (guard.vel.x < 0) guard.vel.x += map_size.x;
		while (guard.vel.y < 0) guard.vel.y += map_size.y;
	}

	return guards;
}


export void day14_1() {
	const auto start_time = high_resolution_clock::now();

	array<int, 4> quadrants{};

	for (auto& guard : file_input()) {
		guard.pos = (guard.pos + 100 * guard.vel) % map_size;

		if (guard.pos.x == half_size.x || guard.pos.y == half_size.y)
			continue;

		const int qx = (guard.pos.x < half_size.x) ? 0 : 1;
		const int qy = (guard.pos.y < half_size.y) ? 0 : 1;
		quadrants[qy * 2 + qx]++;
	}

	const auto safety = ranges::fold_left_first(quadrants, std::multiplies<>());

	const auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start_time);
	println("Day 14a: {} ({})", *safety, duration);
}


static bool search_tree(const auto& map, span<const Guard> guards) {
	for (int x = 20; x < 80; ++x) {
		for (int y = 0; y < 80; ++y) {
			bool tri = true;
			for (int h = 0; tri && h < 5; ++h) {
				if (map[x - h, y + h] != 'x' || map[x + h, y + h] != 'x') {
					tri = false;
					break;
				}
			}

			if (tri) {
				if (false) {
					system("cls");
					static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
					for (auto& guard : guards) {
						SetConsoleCursorPosition(hOut, {(SHORT)guard.pos.x, (SHORT)guard.pos.y});
						print("x");
					}
				}
				return true;
			}
		}
	}
	return false;
}


export void day14_2() {
	const auto start_time = high_resolution_clock::now();

	auto guards = file_input();

	int seconds = 1;
	for (;; ++seconds) {
		array<char, 101 * 103> grid {};
		mdspan<char, extents<size_t, 101, 103>, layout_left> map(grid.data());

		for (auto& guard : guards) {
			guard.pos = (guard.pos + guard.vel) % map_size;

			map[guard.pos.x, guard.pos.y] = 'x';
		}

		if (search_tree(map, guards)) break;
	}

	const auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start_time);
	println("Day 14b: {} ({})", seconds, duration);
}
