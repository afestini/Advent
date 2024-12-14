export module day15;

import std;
import utils;
import <conio.h>;

using namespace std;
using namespace std::chrono;


static Vec2i move2dir(char move) {
	switch (move) {
	case '<': return {-1,0};
	case '>': return {1,0};
	case '^': return {0,-1};
	case 'v': return {0,1};
	default: return {};
	}
}


struct Map {
	vector<string> grid;
	size_t width = 0;
	size_t height = 0;
	string movements;
	Vec2i robot {};

	bool is_wall(Vec2i pos) const { return grid[pos.y][pos.x] == '#'; }
	bool is_box(Vec2i pos) const { return grid[pos.y][pos.x] == 'O'; }
	bool is_box2(Vec2i pos) const { return grid[pos.y][pos.x] == '[' || grid[pos.y][pos.x] == ']'; }
	bool is_free(Vec2i pos) const { return grid[pos.y][pos.x] == '.'; }
	bool is(Vec2i pos, char c) const { return grid[pos.y][pos.x] == c; }

	void init_robot() {
		for (robot.y = 0; robot.y < grid.size(); ++robot.y) {
			if (auto x = grid[robot.y].find('@'); x != string::npos) {
				robot.x = static_cast<int>(x);
				break;
			}
		}
	}

	void scale() {
		for (auto& line : grid) {
			string wider;
			wider.reserve(2 * line.size());
			for (char c : line) {
				if (c == 'O') wider.append("[]");
				else if (c == '@') wider.append("@.");
				else {
					wider.push_back(c);
					wider.push_back(c);
				}
			}

			swap(line, wider);
		}

		init_robot();
	}

	void move(char mv) {
		const auto dir = move2dir(mv);
		auto pos = robot + dir;
		if (is_wall(pos)) return;

		int shifted = 0;
		while (!is_free(pos)) {
			if (is_wall(pos)) return;
			if (is_box(pos)) ++shifted;
			pos += dir;
		}

		grid[robot.y][robot.x] = '.';
		robot += dir;
		grid[robot.y][robot.x] = '@';

		pos = robot + dir;
		for (int i = 0; i < shifted; ++i) {
			grid[pos.y][pos.x] = 'O';
			pos += dir;
		}
	}


	bool is_blocked(int x0, int x1, int y, int dy) {
		for (Vec2i pos {x0, y}; pos.x <= x1; ++pos.x) {
			if (is_wall(pos)) return true;

			if (is(pos, '[')) {
				if (is_blocked(pos.x, pos.x + 1, pos.y + dy, dy)) return true;
			}
			if (is(pos, ']')) {
				if (is_blocked(pos.x - 1, pos.x, pos.y + dy, dy)) return true;
			}
		}
		return false;
	}


	void push(int x0, int x1, int y, int dy) {
		for (Vec2i pos {x0, y}; pos.x <= x1; ++pos.x) {
			if (is(pos, '[')) {
				push(pos.x, pos.x + 1, pos.y + dy, dy);
			}
			if (is(pos, ']')) {
				push(pos.x - 1, pos.x, pos.y + dy, dy);
			}
		}
		for (Vec2i pos {x0, y}; pos.x <= x1; ++pos.x) {
			if (is(pos, '[')) {
				grid[pos.y][pos.x] = '.';
				grid[pos.y][pos.x + 1] = '.';
				grid[pos.y + dy][pos.x] = '[';
				grid[pos.y + dy][pos.x + 1] = ']';
			}
			if (is(pos, ']')) {
				grid[pos.y][pos.x - 1] = '.';
				grid[pos.y][pos.x] = '.';
				grid[pos.y + dy][pos.x - 1] = '[';
				grid[pos.y + dy][pos.x] = ']';
			}
		}
	}


	void move2(char mv) {
		// Nothing changed for left/right
		const auto dir = move2dir(mv);
		auto pos = robot + dir;

		if (dir.y == 0) {
			if (is_wall(pos)) return;

			int shifted = 0;
			while (!is_free(pos)) {
				if (is_wall(pos)) return;
				if (is_box2(pos)) ++shifted;
				pos += dir;
			}

			for (Vec2i dst = robot + (shifted + 1) * dir; dst != robot; dst -= dir) {
				grid[dst.y][dst.x] = grid[dst.y][dst.x - dir.x];
			}
			grid[robot.y][robot.x] = '.';
			robot += dir;
			return;
		}

		if (is_blocked(pos.x, pos.x, pos.y, dir.y)) return;

		push(pos.x, pos.x, pos.y, dir.y);

		grid[robot.y][robot.x] = '.';
		robot += dir;
		grid[robot.y][robot.x] = '@';
	}

	void print() {
		system("cls");
		for (auto& line : grid) println("{}", line);
		println();
	}
};

static Map file_input() {
	ifstream input("day15.txt");

	Map map;

	while (getline(input, map.grid.emplace_back())) {
		if (map.grid.back().empty()) {
			map.grid.pop_back();
			break;
		}
	}

	string line;
	while (getline(input, line)) {
		map.movements.append(line);
	}

	map.width = map.grid[0].size();
	map.height = map.grid.size();
	map.init_robot();

	return map;
}


export void day15_1() {
	const auto start_time = high_resolution_clock::now();

	auto map = file_input();

	for (char move : map.movements) {
		map.move(move);
	}
	//map.print();

	size_t sum = 0;
	for (Vec2i pos {}; pos.y < ssize(map.grid); ++pos.y) {
		for (pos.x = 0; pos.x < ssize(map.grid[pos.y]); ++pos.x) {
			if (map.is_box(pos)) sum += 100 * pos.y + pos.x;
		}
	}

	const auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start_time);
	println("Day 15a: {} ({})", sum, duration);
}


export void day15_2() {
	const auto start_time = high_resolution_clock::now();

	auto map = file_input();

	map.scale();
	map.print();

	for (char move : map.movements) {
	//for (;;){
		/*auto move = _getch();
		if (move == 'j') move = '<';
		if (move == 'i') move = '^';
		if (move == 'l') move = '>';
		if (move == 'k') move = 'v';*/
		map.move2(move);
		//map.print();
	}
	//map.print();

	size_t sum = 0;
	for (Vec2i pos {}; pos.y < ssize(map.grid); ++pos.y) {
		for (pos.x = 0; pos.x < ssize(map.grid[pos.y]); ++pos.x) {
			if (map.grid[pos.y][pos.x] == '[') sum += 100 * pos.y + pos.x;
		}
	}

	const auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start_time);
	println("Day 15b: {} ({})", sum, duration);
}
