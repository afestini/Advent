export module day11;

import std;

using namespace std;
using namespace std::chrono;


static vector<uint64_t> file_input() {
	ifstream input("day11.txt");

	vector<uint64_t> stones;

	while (input) input >> stones.emplace_back();
	stones.pop_back();

	return stones;
}


static int digit_count(int64_t n) {
	int count = 1;
	while (n /= 10) ++count;
	return count;
}

struct Hash11 {
	size_t operator()(pair<uint64_t, int> a) const { return a.first ^ a.second; };
};
unordered_map<pair<uint64_t, int>, uint64_t, Hash11> lookup;


static uint64_t blink(uint64_t stone, int recs) {
	if (recs-- == 0) return 1;

	auto [val, is_new] = lookup.try_emplace({stone, recs});
	if (!is_new) return val->second;

	if (stone == 0) return val->second = blink(1, recs);

	const auto digits = digit_count(stone);
	if (digits % 2 == 0) {
		const auto pow10 = static_cast<int64_t>(pow(10, digits / 2));
		return val->second = blink(stone / pow10, recs) + blink(stone % pow10, recs);
	}

	return val->second = blink(stone * 2024, recs);
}


export void day11_1() {
	const auto start_time = high_resolution_clock::now();

	const auto stones = ranges::fold_left(file_input(), 0ULL,
						[](auto sum, auto stone) {return sum + blink(stone, 25); });

	const auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start_time);
	println("Day 11a: {} ({})", stones, duration);
}


export void day11_2() {
	const auto start_time = high_resolution_clock::now();

	const auto stones = ranges::fold_left(file_input(), 0ULL,
						[](auto sum, auto stone) {return sum + blink(stone, 75); });

	const auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start_time);
	println("Day 11b: {} ({})", stones, duration);
}
