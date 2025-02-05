export module day20;

import std;
import utils;

using namespace std;
using namespace std::chrono;


static constexpr array<Map2D::Pos, 4> directions {{{-1,0}, {0,-1}, {0,1}, {1,0}}};


Vec2i get_start(const Map2D& map) {
	for (Vec2i pos{}; pos.y < map.height; ++pos.y) {
		for (pos.x = 0; pos.x < map.width; ++pos.x) {
			if (map[pos] == 'S') return pos;
		}
	}
	return {};
}


static void fill_cost(const Map2D& map, auto& costs) {
	Vec2i pos = get_start(map);

	int64_t cost = 0;
	costs[pos.x, pos.y] = cost;

	while (map[pos] != 'E') {
		for (const auto dir : directions) {
			const auto next_pos = pos + dir;
			if (costs[next_pos.x, next_pos.y] < 0 && (map[next_pos] == '.' || map[next_pos] == 'E')) {
				pos = next_pos;
				costs[next_pos.x, next_pos.y] = ++cost;
				break;
			}
		}
	}
}


export void day20_1() {
	const auto start_time = high_resolution_clock::now();

	Map2D map("day20.txt");

	vector<int64_t> costs_data(map.width*map.height, -1);
	mdspan<int64_t, extents<size_t, dynamic_extent, dynamic_extent>, layout_left> costs(costs_data.data(), map.width, map.height);

	fill_cost(map, costs);

	int64_t good_cheats = 0;
	constexpr int64_t threshold = 100;

	for (Vec2i pos {1, 1}; pos.y + 1 < map.height; ++pos.y) {
		for (pos.x = 1; pos.x + 1 < map.width; ++pos.x) {
			if (map[pos] != '#') continue;

			const auto cost_left = costs[pos.x - 1, pos.y];
			const auto cost_right = costs[pos.x + 1, pos.y];
			const auto cost_up = costs[pos.x, pos.y - 1];
			const auto cost_down = costs[pos.x, pos.y + 1];

			good_cheats += cost_left >= 0 && cost_right >= 0 && abs(cost_left - cost_right) >= threshold + 2;
			good_cheats += cost_up >= 0 && cost_down >= 0 && abs(cost_up - cost_down) >= threshold + 2;
		}
	}

	const auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start_time);
	println("Day 20a: {} ({})", good_cheats, duration);
}


export void day20_2() {
	const auto start_time = high_resolution_clock::now();

	Map2D map("day20.txt");

	constexpr int64_t cheat_duration = 20;
	constexpr int64_t threshold = 100;

	vector<int64_t> costs_data(map.width * map.height, -1);
	mdspan<int64_t, extents<size_t, dynamic_extent, dynamic_extent>, layout_left> costs(costs_data.data(), map.width, map.height);

	fill_cost(map, costs);

	int64_t cheat_count = 0;

	for (Vec2i from {0,1}; from.y < map.height - 1; ++from.y) {
		for (from.x = 1; from.x < map.width - 1; ++from.x) {
			const auto from_cost = costs[from.x, from.y];
			if (from_cost < 0) continue;

			for (Vec2i to {0,from.y - cheat_duration}; to.y <= from.y + cheat_duration; ++to.y) {
				const auto dy = abs(to.y - from.y);
				for (to.x = from.x - cheat_duration + dy; to.x <= from.x + cheat_duration - dy; ++to.x) {
					if (!map.is_in_bounds(to)) continue;

					// What about walls? Their cost is -1, so they are failing the test anyway
					// and the extra if costs more performance than doing the calculations for them
					const auto to_cost = costs[to.x, to.y];
					const auto distance = abs(from.x - to.x) + dy;
					const auto saved = to_cost - from_cost - distance;
					if (saved >= threshold && distance <= cheat_duration)
						++cheat_count;
				}
			}
		}
	}

	const auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start_time);
	println("Day 20b: {} ({})", cheat_count, duration);
}


/* Less efficient, but simpler aproach */

static vector<Vec2i> find_path(const Map2D& map) {
	Vec2i prev_pos {-1,-1};
	Vec2i pos = get_start(map);

	vector<Vec2i> path{pos};

	while (map[pos] != 'E') {
		for (const auto dir : directions) {
			const auto next_pos = pos + dir;
			if (next_pos != prev_pos && (map[next_pos] == '.' || map[next_pos] == 'E')) {
				prev_pos = pos;
				pos = next_pos;
				path.emplace_back(next_pos);
				break;
			}
		}
	}
	return path;
}


export void day20_2_alternative() {
	const auto start_time = high_resolution_clock::now();

	Map2D map("day20.txt");

	const auto path = find_path(map);

	constexpr int64_t cheat_duration = 20;
	constexpr int64_t threshold = 100;
	int64_t cheat_count = 0;

	for (size_t i = 0; i < path.size(); ++i) {
		for (size_t k = i + threshold; k < path.size(); ++k) {
			const auto distance = abs(path[i].x - path[k].x) + abs(path[i].y - path[k].y);
			const auto saved = k - i - distance;
			if (saved >= threshold && distance <= cheat_duration)
				++cheat_count;
		}
	}

	const auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start_time);
	println("Day 20b alternnative: {} ({})", cheat_count, duration);
}
