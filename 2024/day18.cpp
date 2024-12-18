export module day18;

import std;
import utils;

using namespace std;
using namespace std::chrono;


static auto file_input() {
	ifstream input("day18.txt");

	string line;
	vector<Vec2i> bytes;
	while (getline(input, line)) {
		auto& vec = bytes.emplace_back();
		for (auto [idx, coord] : views::split(line, ',') | views::enumerate) {
			string_view c(coord);
			from_chars(c.data(), to_address(c.end()), idx ? vec.y : vec.x);
		}
	}
	return bytes;
}


struct Step {
	Vec2i pos;
	int cost;
	int estimate;

	bool operator<(const Step& s) const {
		return cost + estimate > s.cost + s.estimate;
	}
};


static int manhattan(Vec2i goal, Vec2i pos) {
	const auto delta = goal - pos;
	return abs(delta.x) + abs(delta.y);
}


static int find_goal(Map2D& map, Vec2i start, Vec2i goal) {
	constexpr array<Map2D::Pos, 4> directions {{{-1,0}, {0,-1}, {0,1}, {1,0}}};

	priority_queue<Step> q;
	q.emplace(start, 0, manhattan(start, goal));

	while (!q.empty()) {
		const Step step = move(q.top());
		q.pop();

		if (step.pos == goal) return step.cost;
		if (map[step.pos] == 'x') continue;

		map[step.pos] = 'x';

		for (auto dir : directions) {
			const auto next_pos = step.pos + dir;
			if (map.is_in_bounds(next_pos) && map[next_pos] != '#' && map[next_pos] != 'x') {
				q.emplace(next_pos, step.cost + 1, manhattan(goal, next_pos));
			}
		}
	}
	return 0;
}


export void day18_1() {
	const auto start_time = high_resolution_clock::now();

	Map2D map(71, 71);
	const auto bytes = file_input();

	for (auto coord : bytes | views::take(1024))
		map[coord] = '#';

	const auto cost = find_goal(map, Vec2i(0,0), Vec2i(70, 70));

	const auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start_time);
	println("Day 18a: {} ({})", cost, duration);
}


export void day18_2() {
	const auto start_time = high_resolution_clock::now();

	Map2D map(71, 71);
	const auto bytes = file_input();

	size_t byte_count = bytes.size() / 2;
	auto step = byte_count / 2;
	Vec2i final_byte {};

	for (;;) {
		ranges::fill(map.grid, '.'); // Reset the map
		for (auto coord : bytes | views::take(byte_count))
			map[coord] = '#';

		const bool found = find_goal(map, Vec2i(0, 0), Vec2i(70, 70)) > 0;
		if (found) byte_count += step;
		else byte_count -= step; // Do it first, conveniently changes count to index

		if (!found && step == 1) {
			final_byte = bytes[byte_count];
			break;
		}
		step = max(1ULL, (step + 1) / 2);
	}

	const auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start_time);
	println("Day 18b: {},{} ({})", final_byte.x, final_byte.y, duration);
}
