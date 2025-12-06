export module day6;

import std;
import utils;

using namespace std;
using namespace std::chrono;


export void day6_1() {
	const auto start_time = high_resolution_clock::now();

	Map2D in("day6.txt");

	uint64_t sum = 0;
	vector<uint64_t> values;

	const auto op_row = in.Height() - 1;
	for (size_t col_offset = 0; col_offset < in.Width();) {
		auto end_col = col_offset + 1;
		while (end_col < in.Width() && in[end_col, op_row] == ' ') ++end_col;

		for (size_t row = 0; row < op_row; ++row) {
			values.emplace_back(strtoull(&in[col_offset, row], nullptr, 10));
		}

		const char op = in[col_offset, op_row];
		if (op == '+')
			sum += ranges::fold_left(values, 0ULL, plus<>());
		else if (op == '*')
			sum += ranges::fold_left(values, 1ULL, multiplies<>());

		values.clear();
		col_offset = end_col;
	}

	const auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start_time);
	println("Day 6a: {} ({})", sum, duration);
}


export void day6_2() {
	const auto start_time = high_resolution_clock::now();

	Map2D in("day6.txt");

	uint64_t sum = 0;
	vector<uint64_t> values;

	for (size_t col = in.Width() - 1; col < in.Width(); --col) {
		uint64_t value = 0;
		for (size_t row = 0; row < in.Height() - 1; ++row) {
			if (in[col, row] != ' ') {
				value = value * 10 + (in[col, row] - '0');
			}
		}
		values.emplace_back(value);

		const char op = in[col, in.Height() - 1];
		if (op == ' ') continue;

		if (op == '+')
			sum += ranges::fold_left(values, 0ULL, plus<>());
		else if(op == '*')
			sum += ranges::fold_left(values, 1ULL, multiplies<>());

		values.clear();
		--col; // Skip previous column (must be empty)
	}

	const auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start_time);
	println("Day 6b: {} ({})", sum, duration);
}
