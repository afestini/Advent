export module day12;

import std;
import utils;

using namespace std;
using namespace std::chrono;


static constexpr array<Map2D::Pos, 4> directions {{{-1,0}, {0,-1}, {0,1}, {1,0}}};

struct Plot {
	int tiles = 0;
	int fence_pieces = 0;
	int edges = 0;
};


static bool is_different(const Map2D& map, char type, Vec2i pos2) {
	return !map.is_in_bounds(pos2) || (map[pos2] & 0x7F) != type;
}


static void fill_plot(Map2D& map, Map2D::Pos pos, Plot& plot, bool count_edges) {
	if (map[pos] & 0x80) return;

	const auto plant = map[pos];
	map[pos] |= 0x80;
	++plot.tiles;

	if (count_edges) {
		const auto top = pos + Vec2i {0, -1};
		const auto left = pos + Vec2i {-1, 0};
		const auto top_left = pos + Vec2i {-1, -1};
		const auto bottom = pos + Vec2i {0, 1};
		const auto bottom_left = pos + Vec2i {-1, 1};
		const auto right = pos + Vec2i {1, 0};
		const auto top_right = pos + Vec2i {1, -1};

		plot.edges += is_different(map, plant, top) && (is_different(map, plant, left) || !is_different(map, plant, top_left));
		plot.edges += is_different(map, plant, bottom) && (is_different(map, plant, left) || !is_different(map, plant, bottom_left));
		plot.edges += is_different(map, plant, left) && (is_different(map, plant, top) || !is_different(map, plant, top_left));
		plot.edges += is_different(map, plant, right) && (is_different(map, plant, top) || !is_different(map, plant, top_right));
	}

	for (auto dir : directions) {
		const auto neighbor = pos + dir;
		if (!is_different(map, plant, neighbor))
			fill_plot(map, neighbor, plot, count_edges);
		else
			++plot.fence_pieces;
	}
}


export void day12_1() {
	const auto start_time = high_resolution_clock::now();

	Map2D map("day12.txt");

	uint64_t price = 0;

	for (Map2D::Pos pos{0,0}; pos.y < map.height; ++pos.y) {
		for (pos.x = 0; pos.x < map.width; ++pos.x) {
			if ((map[pos] & 0x80) == 0) {
				Plot plot;
				fill_plot(map, pos, plot, false);
				price += plot.tiles * plot.fence_pieces;
			}
		}
	}

	const auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start_time);
	println("Day 12a: {} ({})", price, duration);
}


export void day12_2() {
	const auto start_time = high_resolution_clock::now();

	Map2D map("day12.txt");

	uint64_t price = 0;

	for (Map2D::Pos pos {0,0}; pos.y < map.height; ++pos.y) {
		for (pos.x = 0; pos.x < map.width; ++pos.x) {
			if ((map[pos] & 0x80) == 0) {
				Plot plot;
				fill_plot(map, pos, plot, true);
				price += plot.tiles * plot.edges;
			}
		}
	}

	const auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start_time);
	println("Day 12b: {} ({})", price, duration);
}
