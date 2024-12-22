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


static int64_t mix(int64_t secret, int64_t value) { return secret ^ value; }

static int64_t prune(int64_t secret) { return secret & 0xFFFFFF; }


export void day22_1() {
	const auto start_time = high_resolution_clock::now();

	int64_t sum = 0;
	for (auto secret : file_input()) {
		for (int i = 0; i < 2000; ++i) {
			secret = prune(mix(secret, secret * 64));
			secret = prune(mix(secret, secret / 32));
			secret = prune(mix(secret, secret * 2048));
		}
		sum += secret;
	}

	const auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start_time);
	println("Day 22a: {} ({})", sum, duration);
}


#define USE_HASH 0

#if not USE_HASH
export void day22_2() {
	const auto start_time = high_resolution_clock::now();
	
	// Deltas can only be -9 to +9 and fit into 5 bits.
	// A sequence being only 20 bits allows using a vector
	// instead of a hash map. In total we need 2 * 2 * 1.048.576 bytes
	// Note: could be compressed more (17 bits, 131.072 entries), but is about 25% slower
	constexpr auto seq_size = (1<<20);
	vector<int16_t> global_prices(seq_size);
	vector<int16_t> seen(seq_size);

	for (int16_t id = 0; auto secret : file_input()) {
		int8_t prev_price = secret % 10;
		uint32_t sequence = 0;
		++id;

		for (int i = 1; i <= 2000; ++i) {
			secret = (secret ^ (secret << 6)) & 0xFFFFFF;
			secret = (secret ^ (secret >> 5)) & 0xFFFFFF;
			secret = (secret ^ (secret << 11)) & 0xFFFFFF;
			const int8_t price = secret % 10;

			const uint8_t diff = price - prev_price;
			prev_price = price;

			sequence <<= 5;
			sequence |= (diff < 0 ? 9 - diff : diff) & 0x1F;
			sequence &= 0xFFFFF;

			if (i > 3 && seen[sequence] < id) {
				global_prices[sequence] += price;
				seen[sequence] = id;
			}
		}
	}

	const auto max_price = ranges::max(global_prices);

	const auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start_time);
	println("Day 22b: {} ({})", max_price, duration);
}

#else

export void day22_2() {
	const auto start_time = high_resolution_clock::now();

	unordered_map<uint32_t, int64_t> global_prices;

	for (auto secret : file_input()) {
		int8_t prev_price = secret % 10;
		uint32_t sequence = 0;
		unordered_set<uint32_t> seen;

		for (int i = 1; i <= 2000; ++i) {
			secret = (secret ^ (secret << 6)) & 0xFFFFFF;
			secret = (secret ^ (secret >> 5)) & 0xFFFFFF;
			secret = (secret ^ (secret << 11)) & 0xFFFFFF;
			const int8_t price = secret % 10;

			const uint8_t diff = price - prev_price;
			sequence = (sequence << 8u) | diff;
			prev_price = price;

			if (i > 3 && seen.emplace(sequence).second) {
				global_prices[sequence] += price;
			}
		}
	}

	const auto max_price = ranges::max(global_prices | views::values);

	const auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start_time);
	println("Day 22b: {} ({})", max_price, duration);
}

#endif
