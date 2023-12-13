export module day1;

import std;

using namespace std;


export void day1_1() {
	const auto start = chrono::high_resolution_clock::now();

	ifstream input("day1.txt");
	if (!input) return;

	int64_t sum = 0;
	string line;
	const auto is_digit = [](char ch) {return (bool)isdigit(ch); };
	while (getline(input, line)) {
		if (const auto first_it = ranges::find_if(line, is_digit); first_it != line.end()) {
			const auto last = *ranges::find_if(line | views::reverse, is_digit) - '0';
			sum += (*first_it - '0') * 10 + last;
		}
	}

	const auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
	println("Day 1a: {} ({})", sum, duration);
}




template<typename RangeAdapter>
static int try_get_digit(string_view s, const RangeAdapter& adapter) {
	static constexpr array digits = {"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};

	for (auto pos : views::iota(0, (int)ssize(s)) | adapter) {
		if (isdigit(s[pos])) return s[pos] - '0';

		string_view sub = s.substr(pos);
		for (auto [idx, dig_str] : digits | views::enumerate) {
			if (sub.starts_with(dig_str)) return static_cast<int>(idx);
		}
	}
	return -1;
}

export void day1_2() {
	const auto start = chrono::high_resolution_clock::now();

	ifstream input("day1.txt");
	if (!input) return;

	int64_t sum = 0;
	string line;
	const auto is_digit = [](char ch) {return (bool)isdigit(ch); };
	while (getline(input, line)) {
		const int first = try_get_digit(line, views::all);
		const int last = try_get_digit(line, views::reverse);
		sum += first * 10 + last;
	}

	const auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
	println("Day 1b: {} ({})", sum, duration);
}
