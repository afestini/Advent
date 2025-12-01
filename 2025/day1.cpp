export module day1;

import std;

using namespace std;
using namespace std::chrono;

static generator<int> file_input() {
	ifstream input("day1.txt");
	string line;
	while (getline(input, line)) {
		int distance = 0;
		from_chars(line.data() + 1, to_address(line.end()), distance);
		co_yield (line[0] == 'L') ? -distance : distance;
	}
}

export void day1_1() {
	const auto start_time = high_resolution_clock::now();

	int pos = 50;
	int zeroes = 0;

	for (const int change : file_input()) {
		pos = ((pos + change) % 100 + 100) % 100;

		if (pos == 0) ++zeroes;
	}

	const auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start_time);
	println("Day 1a: {} ({})", zeroes, duration);
}


export void day1_2() {
	const auto start_time = high_resolution_clock::now();

	int pos = 50;
	int zeroes = 0;

	for (const int change : file_input()) {
		if (change > 0) {
			zeroes += (pos + change) / 100;
		}
		else {
			zeroes += (((100 - pos) % 100) - change) / 100;
		}
		pos = ((pos + change) % 100 + 100) % 100;
	}

	const auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start_time);
	println("Day 1b: {} ({})", zeroes, duration);
}
