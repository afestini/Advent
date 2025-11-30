export module day1;

import std;

using namespace std;
using namespace std::chrono;



export void day1_1() {
	const auto start_time = high_resolution_clock::now();

	

	const auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start_time);
	println("Day 1a: {} ({})", 0, duration);
}


export void day1_2() {
	const auto start_time = high_resolution_clock::now();

	

	const auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start_time);
	println("Day 1b: {} ({})", 0, duration);
}
