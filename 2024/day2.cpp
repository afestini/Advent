export module day2;

import std;

using namespace std;


static generator<const vector<int>&> file_input() {
	ifstream input("day2.txt");
	string line;
	vector<int> levels;
	while (getline(input, line)) {
		levels.clear();
		for (const auto num : views::split(line, ' ')) {
			string_view s(num);
			levels.emplace_back();
			from_chars(s.data(), to_address(s.end()), levels.back());
		}
		co_yield levels;
	}
}


const auto is_safe = [](int sign, const auto& p) {
	const auto diff = p[1] - p[0];
	const auto step = abs(diff);
	return (sign * diff > 0 && step >= 1 && step <= 3);
};


export void day2_1() {
	const auto start = chrono::high_resolution_clock::now();

	int safe_count = 0;

	for (const auto& levels : file_input()) {
		const int sign = (levels[1] < levels[0]) ? -1 : 1;
		if (ranges::all_of(views::slide(levels, 2), bind_front(is_safe, sign)))
			++safe_count;
	}

	const auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
	println("Day 2a: {} ({})", safe_count, duration);
}


// With the dampener, looking at just the first pair is no longer working, go by majority (2 out of 3)
static int DetermineSign(span<const int> levels) {
	int up = 0;
	int down = 0;
	for (const auto p : views::slide(levels, 2)) {
		if (p[1] > p[0] && ++up > 1) return 1;
		if (p[1] < p[0] && ++down > 1) return -1;
	}
	return 0;
}


static int CheckReport(span<const int> levels, int sign, int skip) {
	int prev = (skip == 0) ? 1 : 0;
	for (int i = prev + 1; i < levels.size(); i++) {
		if (i == skip) continue;

		if (!is_safe(sign, array {levels[prev], levels[i]})) {
			return max(skip + 1, prev);
		}
		prev = i;
	}
	return 0;
}


export void day2_2() {
	const auto start = chrono::high_resolution_clock::now();

	int safe_count = 0;

	for (const auto& levels : file_input()) {
		const int sign = DetermineSign(levels);

		// Only retry with skipping the two levels involved in an error (skip first value just because it doesn't hurt)
		int fail_at = CheckReport(levels, sign, 0);
		if (fail_at) fail_at = CheckReport(levels, sign, fail_at);
		if (fail_at) fail_at = CheckReport(levels, sign, fail_at);

		if (!fail_at) ++safe_count;
	}
	
	const auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
	println("Day 2b: {} ({})", safe_count, duration);
}
