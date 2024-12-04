export module day4;

import std;
import utils;

using namespace std;


static int IsItXmas(const Map2D& grid, Map2D::Pos s_pos) {
	static constexpr array<Map2D::Pos, 8> directions {{{-1,-1}, {-1,0}, {-1,1}, {0,-1}, {0,1}, {1,-1}, {1,0}, {1,1}}};
	static constexpr string_view word = "XMAS";

	int count = 0;
	for (const auto& dir : directions) {
		size_t idx = 0;
		Map2D::Pos pos = s_pos;
		while (pos.y < grid.Height() && pos.x < grid.Width() && idx < word.size()) {
			if (grid[pos] != word[idx]) break;

			pos += dir;
			if (++idx == word.size()) {
				count++;
			}
		}
	}
	return count;
}

export void day4_1() {
	const auto start = chrono::high_resolution_clock::now();

	const auto grid = Map2D("day4.txt");

	int count = 0;
	for (int y = 0; y < grid.Height(); ++y) {
		for (int x = 0; x < grid.Width(); ++x) {
			count += IsItXmas(grid, Map2D::Pos(x,y));
		}
	}

	const auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
	println("Day 4a: {} ({})", count, duration);
}


static int IsItCrossMas(const Map2D& grid, Map2D::Pos pos) {
	static constexpr array<Map2D::Pos, 8> directions {{{-1,-1}, {-1,1}, {1,-1}, {1,1}}};

	if (grid[pos] != 'A') return 0;

	int count = 0;
	for (const auto& dir : directions) {
		if (grid[pos + dir] == 'M' && grid[pos - dir] == 'S') {
			++count;
		}
	}
	return count == 2 ? 1 : 0;
}


export void day4_2() {
	const auto start = chrono::high_resolution_clock::now();

	const auto grid = Map2D("day4.txt");

	int count = 0;
	for (int y = 1; y + 1 < grid.Height(); ++y) {
		for (int x = 1; x + 1 < grid.Width(); ++x) {
			count += IsItCrossMas(grid, Map2D::Pos(x, y));
		}
	}

	const auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
	println("Day 4b: {} ({})", count, duration);
}
