export module day9;

import std;
import utils;

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

	vector<int> uncompressed;
	deque<pair<size_t, size_t>> free_list;

	for (auto [id, size_space] : file_input() | views::chunk(2) | views::enumerate) {
		uncompressed.append_range(vector<int>(size_space[0] - '0', static_cast<int>(id)));
		if (size_space.size() > 1 && size_space[1] > '0') {
			free_list.emplace_back(uncompressed.size(), size_space[1] - '0');
			uncompressed.append_range(vector<int>(size_space[1] - '0', -1));
		}
	}

	while (!free_list.empty()) {
		auto last_pos = ranges::find_last_if(uncompressed, [](auto n) {return n != -1;});
		auto last = *last_pos.begin();

		auto& free = free_list.front();
		if (free.first >= uncompressed.size()) break;

		uncompressed[free.first++] = last;
		uncompressed.erase(last_pos.begin(), last_pos.end());
		if (--free.second == 0)
			free_list.erase(free_list.begin());
	}

	size_t sum = 0;
	for (auto [idx, id] : uncompressed | views::enumerate) {
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

	deque<BlockInfo> free_list;
	vector<BlockInfo> block_info;
	size_t disk_pos = 0;

	for (auto [id, size_space] : file_input() | views::chunk(2) | views::enumerate) {
	//for (auto [id, size_space] : "2333133121414131402"sv | views::chunk(2) | views::enumerate) {
		block_info.emplace_back(id, disk_pos, static_cast<uint8_t>(size_space[0] - '0'));
		disk_pos += size_space[0] - '0';

		if (size_space.size() > 1 && size_space[1] > '0') {
			free_list.emplace_back(0, disk_pos, static_cast<uint8_t>(size_space[1] - '0'));
			disk_pos += size_space[1] - '0';
		}
	}

	for (auto& info : block_info | views::reverse) {
		auto it = ranges::find_if(free_list, [&info](auto sz) { return sz >= info.size; }, &BlockInfo::size);
		if (it != free_list.end() && it->pos < info.pos) {
			info.pos = it->pos;
			it->pos += info.size;
			it->size -= info.size;

			if (it->size == 0) free_list.erase(it);
		}
	}

	size_t sum = 0;
	for (auto& block : block_info) {
		for (size_t p = block.pos; p < block.pos + block.size; ++p) {
			sum += p * block.id;
		}
	}

	const auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start_time);
	println("Day 9b: {} ({})", sum, duration);
}
