export module day8;

import std;
import utils;

using namespace std;


struct Antenna {
	Vec2 pos;
	char frequency = ' ';
};

struct AntennaMap {
	vector<Antenna> antennas;
	size_t width = 0;
	size_t height = 0;
};

using UniqueLocations = unordered_set<Vec2, Vec2Hash>;


static AntennaMap file_input() {
	ifstream input("day8.txt");

	AntennaMap map;

	string line;
	while (getline(input, line)) {
		for (auto [x, frequency] : line | views::enumerate) {
			if (frequency != '.') {
				map.antennas.emplace_back(Vec2{static_cast<int>(x), static_cast<int>(map.height)}, frequency);
			}
		}
		++map.height;
		map.width = line.size();
	}

	ranges::sort(map.antennas, {}, &Antenna::frequency);
	return map;
}


static void for_all_antenna_pairs(const AntennaMap& map, const function<void(Antenna, Antenna, Vec2)>& func) {
	for (const auto [idx, antenna_a] : map.antennas | views::enumerate) {
		for (const auto& antenna_b : map.antennas | views::drop(idx + 1)) {
			if (antenna_b.frequency != antenna_a.frequency) break;

			func(antenna_a, antenna_b, antenna_b.pos - antenna_a.pos);
		}
	}
}


static bool check_and_add(UniqueLocations& locations, const AntennaMap& map, Vec2 pos) {
	if (static_cast<size_t>(pos.x) >= map.width || static_cast<size_t>(pos.y) >= map.height)
		return false;

	locations.emplace(pos);
	return true;
}


export void day8_1() {
	const auto start = chrono::high_resolution_clock::now();

	const auto map = file_input();

	UniqueLocations unique_locations;

	for_all_antenna_pairs(map, [&map, &unique_locations](Antenna antenna_a, Antenna antenna_b, Vec2 dir) {
		check_and_add(unique_locations, map, antenna_a.pos - dir);
		check_and_add(unique_locations, map, antenna_b.pos + dir);
	});

	const auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
	println("Day 8a: {} ({})", unique_locations.size(), duration);
}


export void day8_2() {
	const auto start = chrono::high_resolution_clock::now();

	const auto map = file_input();

	UniqueLocations unique_locations;

	for_all_antenna_pairs(map, [&map, &unique_locations](Antenna antenna_a, Antenna antenna_b, Vec2 dir) {
		for (auto pos = antenna_a.pos; check_and_add(unique_locations, map, pos); pos -= dir);
		for (auto pos = antenna_b.pos; check_and_add(unique_locations, map, pos); pos += dir);
	});

	const auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
	println("Day 8b: {} ({})", unique_locations.size(), duration);
}
