export module day13;

import std;

using namespace std;

struct Pattern {
	vector<char> data;
	int64_t width;
	int64_t height;
};


static vector<Pattern> ReadInput() {
	ifstream input("day13.txt");

	vector<Pattern> data(1);
	auto* pattern = &data.back();
	string line;
	while (getline(input, line)) {
		if (line.empty()) {
			pattern->height = pattern->data.size() / pattern->width;
			pattern = &data.emplace_back();
		}
		else {
			pattern->width = line.length();
			pattern->data.append_range(line);
		}
	}
	pattern->height = pattern->data.size() / pattern->width;
	return data;
}


static bool IsMirrored(const auto& pattern, int64_t offset, int64_t exp_diffs) {
	const auto size = min(offset, (int64_t)pattern.extent(1) - offset);

	int64_t diffs = 0;
	for (int64_t row = 0; row < pattern.extent(0); ++row) {
		for (auto beg = offset - 1, end = offset, steps = size; steps; --steps, --beg, ++end) {
			if (pattern[array{row, beg}] != pattern[array{row, end}]) {
				if (++diffs > exp_diffs) return false; 
			}
		}
	}
	return diffs == exp_diffs;
}


static int64_t CheckPattern(const auto& pattern, int64_t exp_diffs) {
	for (int64_t offset = 1; offset < pattern.extent(1); ++offset) {
		if (IsMirrored(pattern, offset, exp_diffs)) {
			return offset;
		}
	}
	return 0;
}


static int64_t FindReflections(span<const Pattern> input, int64_t exp_diffs) {
	using row_major = mdspan<const char, extents<int64_t, std::dynamic_extent, std::dynamic_extent>, layout_right>;
	using col_major = mdspan<const char, extents<int64_t, std::dynamic_extent, std::dynamic_extent>, layout_left>;

	int64_t total = 0;

	for (const auto& pattern : input) {
		const auto row_view = row_major(pattern.data.data(), pattern.height, pattern.width);
		total += CheckPattern(row_view, exp_diffs);

		const auto col_view = col_major(pattern.data.data(), pattern.width, pattern.height);
		total += 100 * CheckPattern(col_view, exp_diffs);
	}
	return total;
}


export void day13_1() {
	const auto start = chrono::high_resolution_clock::now();

	const auto input = ReadInput();
	size_t total = FindReflections(input, 0);

	const auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
	println("Day 13a: {} ({})", total, duration);
}


export void day13_2() {
	const auto start = chrono::high_resolution_clock::now();

	const auto input = ReadInput();
	size_t total = FindReflections(input, 1);

	const auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
	println("Day 13b: {} ({})", total, duration);
}
