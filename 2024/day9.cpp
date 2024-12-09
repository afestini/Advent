export module day9;

import std;

using namespace std;
using namespace std::chrono;


static string file_input() {
	ifstream input("day9.txt");

	string line;
	getline(input, line);
	return line;
}


export void day9_1() {
	const auto start_time = high_resolution_clock::now();

	vector<int> memory;
	memory.reserve(100000);

	// Build uncompressed memory
	for (auto [id, size_space] : file_input() | views::chunk(2) | views::enumerate) {
		while (size_space[0]-- > '0') memory.emplace_back(static_cast<int>(id));
		if (size_space.size() > 1) {
			while (size_space[1]-- > '0') memory.emplace_back(-1);
		}
	}

	// Compress memory
	auto free = to_address(ranges::find(memory, -1));
	while (free < to_address(memory.end())) {
		*free = memory.back();
		while (*++free != -1);
		do memory.pop_back(); while (memory.back() == -1);
	}

	// Calculate checksum
	size_t sum = 0;
	for (auto [idx, id] : memory | views::enumerate) {
		sum += idx * id;
	}

	const auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start_time);
	println("Day 9a: {} ({})", sum, duration);
}


struct BlockInfo {
	uint64_t id;
	size_t pos;
	uint8_t size;
};

export void day9_2() {
	const auto start_time = high_resolution_clock::now();

	array<priority_queue<size_t, vector<size_t>, greater<size_t>>, 10> free_list;
	vector<BlockInfo> block_info;
	block_info.reserve(10000);
	size_t disk_pos = 0;

	// Build block and free lists
	for (auto [id, size_space] : file_input() | views::chunk(2) | views::enumerate) {
		block_info.emplace_back(id, disk_pos, static_cast<uint8_t>(size_space[0] - '0'));
		disk_pos += size_space[0] - '0';

		if (size_space.size() > 1 && size_space[1] > '0') {
			free_list[size_space[1] - '0'].emplace(disk_pos);
			disk_pos += size_space[1] - '0';
		}
	}

	// Compress memory
	for (auto& block : block_info | views::reverse) {
		uint8_t free_size = 0;
		for (uint8_t sz = block.size; sz < 10; ++sz) {
			if (free_list[sz].empty() || free_list[sz].top() >= block.pos) continue;
			if (free_size == 0 || free_list[sz].top() < free_list[free_size].top())
				free_size = sz;
		}
		if (free_size == 0) continue;

		auto free_pos = free_list[free_size].top();

		free_list[free_size].pop();

		block.pos = free_pos;
		free_pos += block.size;
		free_size -= block.size;

		if (free_size > 0) {
			free_list[free_size].emplace(free_pos);
		}
	}

	// Calculate checksum
	size_t sum = 0;
	for (auto& block : block_info) {
		sum += block.id * (block.size * (block.pos + block.pos + block.size - 1)) / 2;
	}

	const auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start_time);
	println("Day 9b: {} ({})", sum, duration);
}
