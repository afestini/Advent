export module day20;

import std;

using namespace std;
using namespace std::chrono;


static auto file_input() {
	ifstream input("day20.txt");

	string line;
	while (getline(input, line)) {
	}

	return line;
}


export void day20_1() {
	const auto start_time = high_resolution_clock::now();


	const auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start_time);
	println("Day 20a: {} ({})", 0, duration);
}


export void day20_2() {
	const auto start_time = high_resolution_clock::now();


	const auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start_time);
	println("Day 20b: {} ({})", 0, duration);
}
