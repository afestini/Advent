export module day4;

import std;
import utils;

using namespace std;
using namespace std::chrono;


static int count_neighbors(Map2D& map, int pos, const array<int, 8>& neighbor_deltas) {
	int count = 0;
	for (const auto delta : neighbor_deltas) {
		const auto neighbor = pos + delta;
		if (neighbor >= 0 && neighbor < ssize(map.grid))
			if (map.grid[neighbor] >= '0')
				++count;
	}
	return count;
}


export void day4_1() {
	const auto start_time = high_resolution_clock::now();

	Map2D map("day4.txt");

	const auto dy = static_cast<int>(map.stride);
	const array neighbor_deltas { -dy - 1, -dy, -dy + 1, -1, 1, dy - 1, dy, dy + 1 };

	uint64_t reachable = 0;

	for (int pos = 0; pos < ssize(map.grid); ++pos) {
		if (map.grid[pos] != '@') continue;

		const size_t rolls = count_neighbors(map, pos, neighbor_deltas);

		reachable += (rolls < 4);
	}

	const auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start_time);
	println("Day 4a: {} ({})", reachable, duration);
}


static int update_neighbors(Map2D& map, int pos, const array<int, 8>& neighbor_deltas) {
	int removed = 0;
	for (const auto delta : neighbor_deltas) {
		const auto neighbor = pos + delta;
		if (neighbor >= 0 && neighbor < ssize(map.grid)) {
			if (map.grid[neighbor] > '0') {
				if (--map.grid[neighbor] < '4') {
					map.grid[neighbor] = '.';
					removed += 1 + update_neighbors(map, neighbor, neighbor_deltas);
				}
			}
		}
	}
	return removed;
}


export void day4_2() {
	const auto start_time = high_resolution_clock::now();

	Map2D map("day4.txt");

	const auto dy = static_cast<int>(map.stride);
	const array neighbor_deltas { -dy - 1, -dy, -dy + 1, -1, 1, dy - 1, dy, dy + 1 };

	// Replace all @ with the neighbor count
	for (int pos = 0; pos < ssize(map.grid); ++pos) {
		if (map.grid[pos] == '@')
			map.grid[pos] = '0' + static_cast<char>(count_neighbors(map, pos, neighbor_deltas));
	}

	int removed = 0;
	// Remove all reachable rolls and recursively update all neighbors immediately
	for (int pos = 0; pos < ssize(map.grid); ++pos) {
		if (map.grid[pos] >= '0' && map.grid[pos] < '4') {
			map.grid[pos] = '.';
			removed += 1 + update_neighbors(map, pos, neighbor_deltas);
		}
	}

	const auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start_time);
	println("Day 4b: {} ({})", removed, duration);
}
