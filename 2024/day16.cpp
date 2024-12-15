export module day16;

import std;
import utils;

using namespace std;
using namespace std::chrono;


static constexpr array<Map2D::Pos, 4> directions {{{-1,0}, {0,-1}, {0,1}, {1,0}}};


static Vec2i get_start(const Map2D& map) {
	for (Vec2i pos {}; pos.y < map.height; ++pos.y) {
		for (pos.x = 0; pos.x < map.width; ++pos.x) {
			if (map[pos] == 'S') return pos;
		}
	}
	return {};
}


struct Step1 {
	Vec2i pos;
	Vec2i dir;
	int cost;

	bool operator<(const Step1& s) const {
		return cost > s.cost;
	}
};


static int find_goal_p1() {
	Map2D map("day16.txt");
	priority_queue<Step1> q;
	q.emplace(get_start(map), Vec2i {1,0}, 0);

	while (!q.empty()) {
		auto step = q.top();
		q.pop();

		if (map[step.pos] == 'E') return step.cost;
		map[step.pos] = 'x';

		auto try_step = [&map, &q, &step](Vec2i dir, int cost) {
			const auto next_pos = step.pos + dir;
			if (map[next_pos] != '#' && map[next_pos] != 'x')
				q.emplace(next_pos, dir, step.cost + cost);
		};
		try_step(step.dir, 1);
		try_step(Vec2i {-step.dir.y, step.dir.x}, 1001);
		try_step(Vec2i {step.dir.y, -step.dir.x}, 1001);
	}
	return 0;
}


struct Step2 {
	Vec2i pos;
	Vec2i dir;
	int cost;
	vector<Vec2i> path;

	bool operator<(const Step2& s) const {
		return cost > s.cost;
	}
};

struct Tile {
	map<Vec2i, int> seen_from;
	//Tile* next = nullptr;
};


static pair<int, size_t> find_goal_p2() {
	Map2D map("day16.txt");
	priority_queue<Step2> q;
	q.emplace(get_start(map), Vec2i {1,0}, 0);

	vector<Tile> tiles(map.width * map.height);

	int max_cost = numeric_limits<int>::max();
	set<Vec2i> cheapest;

	while (!q.empty()) {
		auto step = q.top();
		q.pop();

		if (step.cost > max_cost) continue;

		auto& tile = tiles[step.pos.y * map.width + step.pos.x];
		if (tile.seen_from.contains(step.dir) && tile.seen_from[step.dir] < step.cost) continue;
		tile.seen_from[step.dir] = step.cost;

		step.path.emplace_back(step.pos);

		if (map[step.pos] == 'E') {
			max_cost = step.cost;
			cheapest.insert(step.path.begin(), step.path.end());
			continue;
		}

		auto try_step = [&map, &q, &step](Vec2i dir, int cost) {
			const auto next_pos = step.pos + dir;
			if (map[next_pos] != '#')
				q.emplace(next_pos, dir, step.cost + cost, step.path);
		};
		try_step(step.dir, 1);
		try_step(Vec2i {-step.dir.y, step.dir.x}, 1001);
		try_step(Vec2i {step.dir.y, -step.dir.x}, 1001);
	}

	return {max_cost, cheapest.size()};
}


export void day16_1() {
	const auto start_time = high_resolution_clock::now();

	const int cost = find_goal_p1();

	const auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start_time);
	println("Day 16a: {} ({})", cost, duration);
}


export void day16_2() {
	const auto start_time = high_resolution_clock::now();

	const size_t good_spots = find_goal_p2().second;

	const auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start_time);
	println("Day 16b: {} ({})", good_spots, duration);
}
