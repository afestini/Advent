export module day22;

import std;

using namespace std;
using namespace std::chrono;


static auto file_input() {
	ifstream input("day22.txt");

	vector<int64_t> secrets;
	string line;
	while (getline(input, line)) {
		from_chars(line.data(), to_address(line.end()), secrets.emplace_back());
	}

	return secrets;
}


static int64_t& mix(int64_t& secret, int64_t value) { return secret ^= value; }

static void prune(int64_t& secret) { secret &= 0xFFFFFF; }



export void day22_1() {
	const auto start_time = high_resolution_clock::now();

	int64_t sum = 0;
	for (auto secret : file_input()) {
		for (int i = 0; i < 2000; ++i) {
			prune(mix(secret, secret * 64));
			prune(mix(secret, secret / 32));
			prune(mix(secret, secret * 2048));
		}
		sum += secret;
	}

	const auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start_time);
	println("Day 22a: {} ({})", sum, duration);
}


export void day22_2() {
	const auto start_time = high_resolution_clock::now();

	struct Hash {
		size_t operator()(array<int16_t, 4> a) const { return bit_cast<size_t>(a); }
	};
	unordered_map<array<int16_t, 4>, int64_t, Hash> global_prices;

	for (auto secret : file_input()) {
		int64_t prev_price = secret % 10;
		array<int16_t, 4> sequence = {};
		unordered_set<array<int16_t, 4>, Hash> seen;

		for (int i = 1; i <= 2000; ++i) {
			secret = (secret ^ (secret << 6)) & 0xFFFFFF;
			secret = (secret ^ (secret >> 5)) & 0xFFFFFF;
			secret = (secret ^ (secret << 11)) & 0xFFFFFF;
			const auto price = secret % 10;

			ranges::rotate(sequence, sequence.begin() + 1);
			sequence[3] = static_cast<int16_t>(price - prev_price);
			prev_price = price;

			if (i > 3 && seen.emplace(sequence).second)
				global_prices[sequence] += price;
		}
	}

	const auto max_price = ranges::max(global_prices | views::values);

	const auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start_time);
	println("Day 22b: {} ({})", max_price, duration);
}
