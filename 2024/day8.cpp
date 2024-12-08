export module day8;

import std;
import utils;

using namespace std;
using namespace std::chrono;


using Position = Vec2<uint32_t>;

struct Antenna {
	Position pos;
	char frequency = ' ';
};

struct AntennaMap {
	vector<Antenna> antennas;
	uint32_t width = 0;
	uint32_t height = 0;
};

using UniqueLocations = unordered_set<Position, Vec2Hash<uint32_t>>;


static AntennaMap file_input() {
	ifstream input("day8.txt");

	AntennaMap map;

	string line;
	while (getline(input, line)) {
		for (auto [x, frequency] : line | views::enumerate) {
			if (frequency != '.') {
				map.antennas.emplace_back(Position{static_cast<uint32_t>(x), map.height}, frequency);
			}
		}
		++map.height;
		map.width = static_cast<uint32_t>(line.size());
	}

	// Just use a sorted vector. Since we iterate and don't do lookups, multimaps would be worse in every way
	// (and just awkward to work with anyway)
	ranges::sort(map.antennas, {}, &Antenna::frequency);
	return map;
}


// Antenna is 12 bytes, Position is 8. Pass by reference would be counter-productive. It adds overhead for
// the indirection and potentially prevents optimizations due to aliasing rules (also easier on the eyes)
static void for_all_antenna_pairs(const AntennaMap& map, const function<void(Antenna, Antenna, Position)>& func) {
	for (const auto [idx, antenna_a] : map.antennas | views::enumerate) {
		for (const auto& antenna_b : map.antennas | views::drop(idx + 1)) {
			if (antenna_b.frequency != antenna_a.frequency) break;

			func(antenna_a, antenna_b, antenna_b.pos - antenna_a.pos);
		}
	}
}


// Yes, yes.. doing two things in one function. Picking nicer code over dogma here...
static bool check_and_add(UniqueLocations& locations, const AntennaMap& map, Position pos) {
	// Since we're using unsigned, we can skip half the checks
	if (pos.x >= map.width || pos.y >= map.height)
		return false;

	locations.emplace(pos);
	return true;
}


export void day8_1() {
	const auto start_time = high_resolution_clock::now();

	const auto map = file_input();

	UniqueLocations locations;

	for_all_antenna_pairs(map, [&map, &locations](Antenna antenna_a, Antenna antenna_b, Position dir) {
		check_and_add(locations, map, antenna_a.pos - dir);
		check_and_add(locations, map, antenna_b.pos + dir);
	});

	const auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start_time);
	println("Day 8a: {} ({})", locations.size(), duration);
}


export void day8_2() {
	const auto start_time = high_resolution_clock::now();

	const auto map = file_input();

	UniqueLocations locations;

	for_all_antenna_pairs(map, [&map, &locations](Antenna antenna_a, Antenna antenna_b, Position dir) {
		for (auto pos = antenna_a.pos; check_and_add(locations, map, pos); pos -= dir);
		for (auto pos = antenna_b.pos; check_and_add(locations, map, pos); pos += dir);
	});

	const auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start_time);
	println("Day 8b: {} ({})", locations.size(), duration);
}
