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


static bool is_different(const Map2D& map, char type, Vec2i pos) {
	return !map.is_in_bounds(pos) || (map[pos] & 0x7F) != type;
}


static void fill_plot(Map2D& map, Map2D::Pos pos, Plot& plot) {
	if (map[pos] & 0x80) return;

	const auto plant = map[pos];
	map[pos] |= 0x80;
	++plot.tiles;

	// For clarity (handled in loop below).. number of edges = number of corners (vertices)
	// For each fence, check the "left" (or counter-clockwise) end for being an inner or outer corner
	//plot.edges += top_different && (left_different || !is_different(map, plant, top_left));
	//plot.edges += bottom_different && (left_different || !is_different(map, plant, bottom_right));
	//plot.edges += left_different && (top_different || !is_different(map, plant, bottom_left));
	//plot.edges += right_different && (top_different || !is_different(map, plant, top_right));

	for (auto dir : directions) {
		const auto neighbor = pos + dir;
		if (!is_different(map, plant, neighbor)) {
			fill_plot(map, neighbor, plot);
		}
		else {
			++plot.fence_pieces;
			const auto rel_left = pos + Vec2i{-dir.y, dir.x};
			plot.edges += is_different(map, plant, rel_left) || !is_different(map, plant, rel_left + dir);
		}
	}
}


export void day12() {
	const auto start_time = high_resolution_clock::now();

	Map2D map("day12.txt");

	uint64_t price_fences = 0;
	uint64_t price_edges = 0;

	for (Map2D::Pos pos{0,0}; pos.y < map.height; ++pos.y) {
		for (pos.x = 0; pos.x < map.width; ++pos.x) {
			if ((map[pos] & 0x80) == 0) {
				Plot plot;
				fill_plot(map, pos, plot);
				price_fences += plot.tiles * plot.fence_pieces;
				price_edges += plot.tiles * plot.edges;
			}
		}
	}

	const auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start_time);
	println("Day 12a: {} ({})", price_fences, duration);
	println("Day 12b: {} ({})", price_edges, duration);
}
