export module day12;

import std;
import utils;

using namespace std;
using namespace std::chrono;


static constexpr array<Map2D::Pos, 4> directions {{{-1,0}, {0,-1}, {0,1}, {1,0}}};

struct Plot {
	unordered_set<Map2D::Pos, Vec2Hash<int>> tiles;
	int fence_pieces = 0;
};


static bool is_different(const Map2D& map, Vec2i pos, Vec2i pos2) {
	return !map.is_in_bounds(pos2) || map[pos2] != map[pos];
}


static void fill_plot(const Map2D& map, Map2D::Pos pos, Plot& plot) {
	if (plot.tiles.contains(pos)) return;

	plot.tiles.emplace(pos);

	for (auto dir : directions) {
		const auto neighbor = pos + dir;
		if (is_different(map, pos, neighbor))
			++plot.fence_pieces;
		else
			fill_plot(map, neighbor, plot);
	}
}


export void day12_1() {
	const auto start_time = high_resolution_clock::now();

	Map2D map("day12.txt");

	std::map<char, vector<Plot>> plots;
	uint64_t price = 0;

	for (Map2D::Pos pos{0,0}; pos.y < map.height; ++pos.y) {
		for (pos.x = 0; pos.x < map.width; ++pos.x) {
			auto& plots_of_type = plots[map[pos]];
			if (ranges::none_of(plots_of_type, [pos](auto& plot) { return plot.tiles.contains(pos); })) {
				auto& new_plot = plots_of_type.emplace_back();
				fill_plot(map, pos, new_plot);
				price += new_plot.tiles.size() * new_plot.fence_pieces;
			}
		}
	}

	const auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start_time);
	println("Day 12a: {} ({})", price, duration);
}


static int count_edges(const Map2D& map, const Plot& plot) {
	int edges = 0;

	for (auto pos : plot.tiles) {
		const auto top = pos + Vec2i{0, -1};
		const auto left = pos + Vec2i{-1, 0};
		const auto top_left = pos + Vec2i{-1, -1};
		const auto bottom = pos + Vec2i{0, 1};
		const auto bottom_left = pos + Vec2i{-1, 1};
		const auto right = pos + Vec2i{1, 0};
		const auto top_right = pos + Vec2i{1, -1};

		edges += is_different(map, pos, top) && (is_different(map, pos, left) || !is_different(map, pos, top_left));

		edges += is_different(map, pos, bottom) && (is_different(map, pos, left) || !is_different(map, pos, bottom_left));

		edges += is_different(map, pos, left) && (is_different(map, pos, top) || !is_different(map, pos, top_left));
		
		edges += is_different(map, pos, right) && (is_different(map, pos, top) || !is_different(map, pos, top_right));
	}
	return edges;
}


export void day12_2() {
	const auto start_time = high_resolution_clock::now();

	Map2D map("day12.txt");

	std::map<char, vector<Plot>> plots;
	uint64_t price = 0;

	for (Map2D::Pos pos {0,0}; pos.y < map.height; ++pos.y) {
		for (pos.x = 0; pos.x < map.width; ++pos.x) {
			auto& plots_of_type = plots[map[pos]];
			if (ranges::none_of(plots_of_type, [pos](auto& plot) { return plot.tiles.contains(pos); })) {
				auto& new_plot = plots_of_type.emplace_back();
				fill_plot(map, pos, new_plot);
				price += new_plot.tiles.size() * count_edges(map, new_plot);
			}
		}
	}

	const auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start_time);
	println("Day 12b: {} ({})", price, duration);
}
