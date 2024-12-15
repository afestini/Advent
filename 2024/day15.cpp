export module day15;

import std;
import utils;
import <conio.h>;

using namespace std;
using namespace std::chrono;


static Vec2i move2dir(char move) {
	switch (move) {
	case '<': return {-1, 0};
	case '>': return {1, 0};
	case '^': return {0, -1};
	case 'v': return {0, 1};
	default: return {};
	}
}


struct Map {
	vector<string> grid;
	string movements;
	Vec2i robot {};

	Map() {
		ifstream input("day15.txt");

		while (getline(input, grid.emplace_back())) {
			if (grid.back().empty()) {
				grid.pop_back();
				break;
			}
		}

		string line;
		while (getline(input, line)) {
			movements.append(line);
		}
	}


	bool is_box(Vec2i pos) const {
		const auto tile = grid[pos.y][pos.x];
		return tile == 'O' || tile == '[' || tile == ']';
	}

	bool is(Vec2i pos, char c) const { return grid[pos.y][pos.x] == c; }
	bool is(int x, int y, char c) const { return grid[y][x] == c; }


	void init_robot() {
		for (robot.y = 0; robot.y < ssize(grid); ++robot.y) {
			if (auto x = grid[robot.y].find('@'); x != string::npos) {
				robot.x = static_cast<int>(x);
				break;
			}
		}
	}


	void scale() {
		for (auto& line : grid) {
			string scaled;
			for (char c : line) {
				switch (c) {
				case '#': scaled.append("##"); break;
				case '.': scaled.append(".."); break;
				case '@': scaled.append("@."); break;
				case 'O': scaled.append("[]"); break;
				}
			}
			swap(line, scaled);
		}
	}


	void print() {
		system("cls");
		for (auto& line : grid) println("{}", line);
	}


	int gps_sum(char box) {
		int sum = 0;
		for (int y = 0; y < ssize(grid); ++y) {
			for (int x = 0; x < ssize(grid[y]); ++x) {
				if (grid[y][x] == box) sum += 100 * y + x;
			}
		}
		return sum;
	}

	void move_p1(char mv) {
		const auto dir = move2dir(mv);

		auto pos = robot + dir;
		for (; !is(pos, '.'); pos += dir)
			if (is(pos, '#')) return; // Bail, we can't move

		for (; pos != robot; pos -= dir)
			swap(grid[pos.y][pos.x], grid[pos.y - dir.y][pos.x - dir.x]);

		// For historic reasons: part 1 can be much simpler.. set the "end of the line" to a box,
		// and overwrite the new robot position. No point in iterating and actually "moving" anything.
		//	grid[pos.y][pos.x] = 'O';
		//	grid[robot.y + dir.y][robot.x + dir.x] = '@';
		//	grid[robot.y][robot.x] = '.';

		robot += dir;
	}


	bool is_blocked(int x, int y, int dy) {
		const auto tile = grid[y][x];

		if (tile == '.') return false;
		if (tile == '#') return true;

		// Adjust x, so we handle left/right side without further case checks
		if (tile == ']') --x;

		// It's just a single aligned box in front of us, don't split
		if (is(x, y + dy, '[')) return is_blocked(x, y + dy, dy);

		return is_blocked(x, y + dy, dy) || is_blocked(x + 1, y + dy, dy);
	}


	void push(int x, int y, int dy) {
		if (is(x, y, '.')) return;

		// Adjust x, so we handle left/right side without further case checks
		if (is(x, y, ']')) --x;

		push(x, y + dy, dy);
		swap(grid[y + dy][x], grid[y][x]);

		push(x + 1, y + dy, dy);
		swap(grid[y + dy][x + 1], grid[y][x + 1]);
	}


	void move_p2(char mv) {
		const auto dir = move2dir(mv);

		// Sideways didn't change
		if (dir.y == 0) return move_p1(mv);

		const auto next_pos = robot + dir;
		if (!is_blocked(next_pos.x, next_pos.y, dir.y)) {
			push(next_pos.x, next_pos.y, dir.y);

			grid[robot.y][robot.x] = '.';
			robot = next_pos;
			grid[robot.y][robot.x] = '@';
		}
	}

};


export void day15_1() {
	const auto start_time = high_resolution_clock::now();

	Map map;
	map.init_robot();

	for (const char move : map.movements) map.move_p1(move);

	const size_t sum = map.gps_sum('O');

	const auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start_time);
	println("Day 15a: {} ({})", sum, duration);
}


export void day15_2(bool interactive = false) {
	const auto start_time = high_resolution_clock::now();

	Map map;
	map.scale();
	map.init_robot();

	if (interactive) {
		for (;;) {
			map.print();
			auto move = static_cast<char>(_getch());
			if (move == 'i' || move == 'w') move = '^';
			if (move == 'j' || move == 'a') move = '<';
			if (move == 'k' || move == 's') move = 'v';
			if (move == 'l' || move == 'd') move = '>';
			map.move_p2(move);
		}
	}
	else {
		for (const char move : map.movements) map.move_p2(move);
	}

	const size_t sum = map.gps_sum('[');

	const auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start_time);
	println("Day 15b: {} ({})", sum, duration);
}
