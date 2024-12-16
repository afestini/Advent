export module day17;

import std;

using namespace std;
using namespace std::chrono;


static string file_input() {
	ifstream input("day17.txt");

	string line;
	while (getline(input, line)) {
	}
	return line;
}


export void day17_1() {
	const auto start_time = high_resolution_clock::now();



	const auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start_time);
	println("Day 17a: {} ({})", 0, duration);
}


export void day17_2() {
	const auto start_time = high_resolution_clock::now();



	const auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start_time);
	println("Day 17b: {} ({})", 0, duration);
}
