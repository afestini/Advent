export module day3;

import std;

using namespace std;

static constexpr bool use_regex = false;

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


static int64_t EvaluateMulStr(string_view txt, size_t pos) {
	string_view args(txt.data() + pos, txt.find(")", pos) - pos);
	size_t comma = args.find(',');
	string_view left = args.substr(0, comma);
	string_view right = args.substr(comma + 1);
	if (left.size() > 3 || right.size() > 3) return 0;
	int64_t x = 0;
	int64_t y = 0;
	from_chars(to_address(left.begin()), to_address(left.end()), x);
	from_chars(to_address(right.begin()), to_address(right.end()), y);
	return x * y;
}


export void day3_1() {
	const auto start = chrono::high_resolution_clock::now();

	int64_t count = 0;
	for (const auto& txt : file_input()) {
		if constexpr (use_regex) {
			const regex rx(R"(mul\(([\d]{1,3}),([\d]{1,3})\))");
			for (sregex_iterator it(txt.begin(), txt.end(), rx); it != sregex_iterator(); ++it) {
				count += EvaluateMul(*it);
			}
		}
		else {
			for (auto pos = txt.find("mul("); pos != string::npos; pos = txt.find("mul(", pos + 4)) {
				count += EvaluateMulStr(txt, pos + 4);
			}
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
		if constexpr (use_regex) {
			const regex rx(R"(do\(\)|don't\(\)|mul\(([\d]{1,3}),([\d]{1,3})\))");
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
		else {
			size_t mul_pos = txt.find("mul(");
			size_t do_pos = txt.find("do()");
			size_t dont_pos = txt.find("don't()");

			while (mul_pos != string::npos) {
				if (do_pos < min(dont_pos, mul_pos)) {
					enabled = true;
					do_pos = txt.find("do()", do_pos + 4);
				}
				else if (dont_pos < min(do_pos, mul_pos)) {
					enabled = false;
					dont_pos = txt.find("don't()", dont_pos + 7);
				}
				else if (mul_pos < min(do_pos, dont_pos)) {
					if (enabled) {
						count += EvaluateMulStr(txt, mul_pos+4);
					}
					mul_pos = txt.find("mul(", mul_pos+4);
				}
			}
		}
	}

	const auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
	println("Day 3b: {} ({})", count, duration);
}
