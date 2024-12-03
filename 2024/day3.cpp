export module day3;

import std;

using namespace std;


static generator<const string&> file_input() {
	ifstream input("day3.txt");
	string line;
	while (getline(input, line)) {
		co_yield line;
	}
}


static int64_t EvaluateMul(const auto& matches) {
	int64_t x = 0;
	int64_t y = 0;
	from_chars(to_address(matches[1].first), to_address(matches[1].second), x);
	from_chars(to_address(matches[2].first), to_address(matches[2].second), y);
	return x * y;
}


export void day3_1() {
	const auto start = chrono::high_resolution_clock::now();

	int64_t count = 0;
	for (const auto& txt : file_input()) {
		regex rx(R"(mul\(([\d]{1,3}),([\d]{1,3})\))");
		for (sregex_iterator it(txt.begin(), txt.end(), rx); it != sregex_iterator(); ++it) {
			count += EvaluateMul(*it);
		}
	}

	const auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
	println("Day 3a: {} ({})", count, duration);
}


export void day3_2() {
	const auto start = chrono::high_resolution_clock::now();

	bool enabled = true;
	int64_t count = 0;
	for (const auto& txt : file_input()) {
		regex rx(R"(do\(\)|don't\(\)|mul\(([\d]{1,3}),([\d]{1,3})\))");
		for (sregex_iterator it(txt.begin(), txt.end(), rx); it != sregex_iterator(); ++it) {
			if ((*it)[0].compare("do()") == 0) {
				enabled = true;
			}
			else if ((*it)[0].compare("don't()") == 0) {
				enabled = false;
			}
			else if (enabled) {
				count += EvaluateMul(*it);
			}
		}
	}

	const auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
	println("Day 3b: {} ({})", count, duration);
}
