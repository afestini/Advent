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

	vector<int64_t> global_prices(1 << 20);

	for (auto secret : file_input()) {
		int8_t prev_price = secret % 10;
		uint32_t sequence = 0;
		vector<bool> seen(1 << 20);

		for (int i = 1; i <= 2000; ++i) {
			secret = (secret ^ (secret << 6)) & 0xFFFFFF;
			secret = (secret ^ (secret >> 5)) & 0xFFFFFF;
			secret = (secret ^ (secret << 11)) & 0xFFFFFF;
			const int8_t price = secret % 10;

			const auto diff = price - prev_price;
			sequence = ((sequence << 5u) | (diff & 0x1F)) & 0xFFFFF;
			prev_price = price;

			if (i > 3 && !seen[sequence]) {
				global_prices[sequence] += price;
				seen[sequence] = 1;
			}
		}
	}

	const auto max_price = ranges::max(global_prices);

	const auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start_time);
	println("Day 22b: {} ({})", max_price, duration);
}
