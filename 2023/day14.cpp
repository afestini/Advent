export module day14;

import std;

using namespace std;


static vector<string> ReadInput() {
	ifstream input("day14.txt");

	vector<string> lines(1);
	while (getline(input, lines.back())) {
		lines.emplace_back();
	}
	lines.pop_back();

	return lines;
}


static void RollNorthSouth(span<string> input, int dir, const auto& range_adapter) {
	for (int64_t c = 0; c < ssize(input[0]); ++c) {
		int64_t dst = (dir < 0) ? (ssize(input) - 1) : 0;
		for (auto [r, line] : input | views::enumerate | range_adapter) {
			if (line[c] == 'O') {
				swap(input[dst][c], line[c]);
				dst += dir;
			}
			else if (line[c] == '#') {
				dst = r + dir;
			}
		}
	}
}


static void RollEastWest(span<string> input, int dir, const auto& range_adapter) {
	for (auto [r, line] : input | views::enumerate) {
		int64_t dst = (dir < 0) ? (ssize(line) - 1) : 0;
		for (auto [c, ch] : line | views::enumerate | range_adapter) {
			if (ch == 'O') {
				swap(input[r][dst], ch);
				dst += dir;
			}
			else if (ch == '#') {
				dst = c + dir;
			}
		}
	}
}


static int64_t MeasureLoad(span<const string> input) {
	int64_t sum = 0;
	for (auto [row, line] : input | views::reverse | views::enumerate) {
		sum += (row + 1) * ranges::count(line, 'O');
	}
	return sum;
}


export void day14_1() {
	const auto start = chrono::high_resolution_clock::now();

	auto input = ReadInput();

	RollNorthSouth(input, 1, views::all);
	const int64_t sum = MeasureLoad(input);

	const auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
	println("Day 14a: {} ({})", sum, duration);
}


export void day14_2() {
	const auto start = chrono::high_resolution_clock::now();

	auto input = ReadInput();

	vector<vector<string>> history{input};

	for (int64_t i = 0; i < 1'000'000'000; ++i) {
		RollNorthSouth(input, 1, views::all);
		RollEastWest(input, 1, views::all);
		RollNorthSouth(input, -1, views::reverse);
		RollEastWest(input, -1, views::reverse);

		if (auto it = ranges::find(history | views::reverse, input); it != history.rend()) {
			const auto cycle_size = distance(history.rbegin(), it) + 1;
			const auto cycle_start = history.size() - cycle_size;
			const auto remaining_cycles = 1'000'000'000 - cycle_start;
			input = history[cycle_start + (remaining_cycles % cycle_size)];
			break;
		}
		else history.emplace_back(input);
	}

	const int64_t sum = MeasureLoad(input);

	const auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
	println("Day 14b: {} ({})", sum, duration);
}
