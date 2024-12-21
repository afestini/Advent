export module day22;

import std;

using namespace std;
using namespace std::chrono;


static auto file_input() {
	ifstream input("day22.txt");

	string line;
	while (getline(input, line)) {
	}

	return line;
}


export void day22_1() {
	const auto start_time = high_resolution_clock::now();


	const auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start_time);
	println("Day 22a: {} ({})", 0, duration);
}


export void day22_2() {
	const auto start_time = high_resolution_clock::now();


	const auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start_time);
	println("Day 22b: {} ({})", 0, duration);
}
