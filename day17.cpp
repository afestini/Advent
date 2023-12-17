export module day17;

import std;

using namespace std;


static vector<string> ReadInput() {
	ifstream input("day17.txt");

	vector<string> data(1);
	while (getline(input, data.back())) {
		ranges::transform(data.back(), data.back().begin(), [](char c) {return c - '0';});
		data.emplace_back();
	}
	data.pop_back();
	return data;
}


struct Step {
	int x = 0;
	int y = 0;
	int total_loss = 0;
	int8_t dir_steps = 0;
	int8_t dir = 0;

	auto operator<=>(Step other) const { return other.total_loss <=> total_loss; }
};


struct Dirs {
	int dx;
	int dy;
	int8_t id;
	int8_t rev;
};
static constexpr array dirs{Dirs{-1, 0, 0, 1}, Dirs{1, 0, 1, 0}, Dirs{0, -1, 2, 3}, Dirs{0, 1, 3, 2}};


template<int min_steps, int max_steps>
static int Search(const vector<string>& input) {
	const auto width = ssize(input[0]);
	const auto height = ssize(input);

	vector<array<array<bool, max_steps + 1>, 4>> history(width * height);

	priority_queue<Step> to_check;
	to_check.emplace(0,0,0,0,-1);

	while (!to_check.empty()) {
		auto step = to_check.top();
		to_check.pop();

		if (step.x + 1 == width && step.y + 1 == height && step.dir_steps >= min_steps)
			return step.total_loss;

		for (auto d : dirs) {
			if (d.id == step.dir && step.dir_steps >= max_steps) continue;
			if (step.dir >= 0 && d.id != step.dir && step.dir_steps < min_steps) continue;

			const auto x = step.x + d.dx;
			const auto y = step.y + d.dy;
			if (x >= 0 && x < width && y >= 0 && y < height && d.rev != step.dir) {
				const auto steps = static_cast<int8_t>((step.dir == d.id) ? step.dir_steps + 1 : 1);
				const auto idx = y * width + x;

				if (!history[idx][d.id][steps]) {
					const auto loss = step.total_loss + input[y][x];
					to_check.emplace(x, y, loss, steps, d.id);
					history[idx][d.id][steps] = true;
				}
			}
		}
	}
	return -1;
}


export void day17_1() {
	const auto start = chrono::high_resolution_clock::now();

	auto input = ReadInput();
	const auto heat_loss = Search<1, 3>(input);

	const auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
	println("Day 17a: {} ({})", heat_loss, duration);
}


export void day17_2() {
	const auto start = chrono::high_resolution_clock::now();

	auto input = ReadInput();
	const auto heat_loss = Search<4, 10>(input);

	const auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
	println("Day 17b: {} ({})", heat_loss, duration);
}
