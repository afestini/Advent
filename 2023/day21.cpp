export module day21;

import std;

using namespace std;

static vector<string> ReadInput() {
	ifstream input("day21.txt");

	vector<string> data(1);
	while (getline(input, data.back())) {
		data.emplace_back();
	}
	data.pop_back();

	return data;
}


struct Pos { int64_t x, y, steps; };


static void TryQueue(queue<Pos>& Q, span<string> farm, int64_t x, int64_t y, int64_t steps) {
	if (x >= 0 && x < ssize(farm[0]) &&
	    y >= 0 && y < ssize(farm) &&
	    farm[y][x] != '#' && farm[y][x] != 'x')
	{
		Q.emplace(x, y, steps);
		farm[y][x] = 'x'; // Mark as visited
	}
}


static int64_t ValidPlots(vector<string> farm, int64_t start_x, int64_t start_y, int64_t steps) {
	int64_t reachable = 0;

	queue<Pos> Q;
	TryQueue(Q, farm, start_x, start_y, 0);



	while (!Q.empty()) {
		Pos p = Q.front();
		Q.pop();

		if ((p.steps % 2) == (steps % 2)) ++reachable;
		if (p.steps >= steps) continue;

		TryQueue(Q, farm, p.x, p.y - 1, p.steps + 1);
		TryQueue(Q, farm, p.x, p.y + 1, p.steps + 1);
		TryQueue(Q, farm, p.x - 1, p.y, p.steps + 1);
		TryQueue(Q, farm, p.x + 1, p.y, p.steps + 1);
	}
	return reachable;
}


export void day21_1() {
	const auto start = chrono::high_resolution_clock::now();

	auto farm = ReadInput();
	const auto reachable = ValidPlots(farm, ssize(farm[0]) / 2, ssize(farm) / 2, 64);

	const auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
	println("Day 21a: {} ({})", reachable, duration);
}


export void day21_2() {
	const auto start = chrono::high_resolution_clock::now();

	const auto farm = ReadInput();

	const int64_t max_steps = 26501365;
	const int64_t size = ssize(farm);

	const auto half_width = max_steps / size;
	const auto full_odd_tiles = (half_width - 1) * (half_width - 1);
	const auto full_even_tiles = half_width * half_width;

	// Full tiles
	const int64_t reachable_odd = ValidPlots(farm, size / 2, size / 2, max_steps);
	const int64_t reachable_even = ValidPlots(farm, size / 2, size / 2, max_steps - 1);

	auto total = full_odd_tiles * reachable_odd + full_even_tiles * reachable_even;

	// Diamond tips
	const array tips{Pos{size / 2, size - 1}, Pos{size / 2, 0}, Pos{0, size / 2}, Pos{size - 1, size / 2}};
	for (auto t : tips)
		total += ValidPlots(farm, t.x, t.y, size - 1);

	// Small corners
	const array corners{Pos{0, size - 1}, Pos{0, 0}, Pos{size - 1, size - 1}, Pos{size - 1, 0}};

	for (auto c : corners)
		total += half_width * ValidPlots(farm, c.x, c.y, size / 2 - 1);

	// Large corners
	for (auto c : corners)
		total += (half_width - 1) * ValidPlots(farm, c.x, c.y, size * 3 / 2 - 1);

	const auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
	println("Day 21b: {} ({})", total, duration);
}
