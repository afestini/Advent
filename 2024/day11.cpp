export module day11;

import std;

using namespace std;
using namespace std::chrono;


static void file_input() {
	ifstream input("day8.txt");

	string line;
	while (getline(input, line)) {
	}
}


export void day11_1() {
	const auto start_time = high_resolution_clock::now();



	const auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start_time);
	println("Day 11a: {} ({})", 0, duration);
}



export void day11_2() {
	const auto start_time = high_resolution_clock::now();



	const auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start_time);
	println("Day 11b: {} ({})", 0, duration);
}
