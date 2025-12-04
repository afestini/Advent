export module day2;

import std;
import utils;

using namespace std;
using namespace std::chrono;



static generator<pair<uint64_t, uint64_t>> file_input() {
	ifstream input("day2.txt");
	string line;
	while (getline(input, line)) {
		for (const auto r : views::split(line, ',') | views::transform([](const auto& r) { return string_view(r); })) {
			const auto from = str_as<uint64_t>(r.substr(0, r.find('-')));
			const auto to = str_as<uint64_t>(r.substr(r.find('-') + 1));
			co_yield{ from, to };
		}
	}
}


static uint64_t count_digits(uint64_t n) {
	uint64_t count = 1;
	while (n /= 10) {
		++count;
	}
	return count;
}

static uint64_t base_10_lshift(uint64_t n, uint64_t shift) {
	while (shift--) {
		n *= 10;
	}
	return n;
}

static uint64_t base_10_rshift(uint64_t n, uint64_t shift) {
	while (shift--) {
		n /= 10;
	}
	return n;
}


export void day2_1() {
	const auto start_time = high_resolution_clock::now();

	uint64_t sum = 0;

	for (const auto [from, to] : file_input()) {
		const auto digit_count = (count_digits(from) + 1) / 2;
#if 0
		const auto start_n = base_10_rshift(from, digit_count);
		const auto start_digit_count = count_digits(start_n);
		uint64_t factor = base_10_lshift(1, start_digit_count);

		for (uint64_t n = start_n; ; ++n) {
			if (n >= factor) factor *= 10;

			const auto candidate = n * factor + n;
			if (candidate > to) break;

			if (candidate >= from)
				sum += candidate;
		}
#else
		for (uint64_t n = base_10_rshift(from, digit_count); ; ++n) {
			const auto candidate = n + base_10_lshift(n, count_digits(n));
			if (candidate > to) break;

			if (candidate >= from)
				sum += candidate;
		}
#endif
	}

	const auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start_time);
	println("Day 2a: {} ({})", sum, duration);
}


export void day2_2() {
	const auto start_time = high_resolution_clock::now();

	uint64_t sum = 0;
	unordered_set<uint64_t> invalid;

	for (const auto [from, to] : file_input()) {
		const auto from_digits = count_digits(from);
		const auto to_digits = count_digits(to);

		for (uint64_t pattern_len = 1; pattern_len <= to_digits / 2; ++pattern_len) {
			for (uint64_t repeats = max(2LLU, from_digits / pattern_len); repeats <= to_digits / pattern_len; ++repeats) {
				uint64_t step = 0;
				for (uint64_t i = 0; i < repeats; ++i) {
					step = base_10_lshift(step, pattern_len) + 1;
				}

				const auto max_m = min(to + 1, base_10_lshift(1, pattern_len) * step);
				for (uint64_t candidate = base_10_lshift(1, pattern_len - 1) * step; candidate < max_m; candidate += step) {
					if (candidate >= from && !invalid.contains(candidate)) {
						sum += candidate;
						invalid.insert(candidate);
					}
				}
			}
		}
	}

	const auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start_time);
	println("Day 2b: {} ({})", sum, duration);
}
