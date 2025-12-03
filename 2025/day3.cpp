export module day3;

import std;

using namespace std;
using namespace std::chrono;


static generator<vector<uint32_t>> file_input() {
	ifstream input("day3.txt");
	string line;
	while (getline(input, line)) {
		co_yield views::transform(line, [](char n) { return n - '0'; }) | ranges::to<vector<uint32_t>>();
	}
}


export void day3_1() {
	const auto start_time = high_resolution_clock::now();

	uint64_t sum = 0;

	for (const auto& bank : file_input()) {
		const auto max = ranges::max_element(bank.begin(), bank.end() - 1);
		const auto max2 = ranges::max_element(max + 1, bank.end());
		sum += *max * 10 + *max2;
	}

	const auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start_time);
	println("Day 3a: {} ({})", sum, duration);
}


export void day3_2() {
	const auto start_time = high_resolution_clock::now();

	uint64_t sum = 0;

	for (const auto& bank : file_input()) {
		uint64_t joltage = 0;

		auto it = bank.begin();
		for (int b = 0; b < 12; ++b) {
			it = ranges::max_element(it, bank.end() - (11 - b));
			joltage *= 10;
			joltage += *it++;
		}

		sum += joltage;
	}

	const auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start_time);
	println("Day 3b: {} ({})", sum, duration);
}
