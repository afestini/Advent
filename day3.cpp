export module day3;

import std;

using namespace std;


vector<string> ReadSchematic() {
	ifstream input("day3.txt");
	if (!input) return {};

	vector<string> schematic;
	string line;
	while (getline(input, line)) {
		schematic.emplace_back(move(line));
	}

	return schematic;
}


static bool IsSymbol(char ch) {
	return !isdigit(ch) && ch != '.';
}


struct NumberPos { int64_t first; int64_t last; };


static NumberPos GetNextNumber(string_view line, int64_t pos) {
	while (pos < ssize(line) && !isdigit(line[pos])) ++pos;

	NumberPos pos_len{pos, pos};
	if (pos < ssize(line)) {
		while (pos < ssize(line) && isdigit(line[pos])) ++pos;
		pos_len.last = pos - 1;
	}
	return pos_len;
}


static bool ScanLine(string_view line, int64_t left, int64_t right) {
	const auto segment = line.substr(max(0LL, left), right - left + 1);
	return ranges::any_of(segment, IsSymbol);
}


static int GetPartNumber(const vector<string>& schematic, int64_t line_nr, NumberPos num_pos) {
	const auto left = num_pos.first - 1;
	const auto right = num_pos.last + 1;

	bool is_part_number = (left >= 0 && IsSymbol(schematic[line_nr][left]))
	                   || (right < ssize(schematic[line_nr]) && IsSymbol(schematic[line_nr][right]));

	if (!is_part_number && line_nr > 0)
		is_part_number = ScanLine(schematic[line_nr - 1], left, right);

	if (!is_part_number && line_nr + 1 < ssize(schematic))
		is_part_number = ScanLine(schematic[line_nr + 1], left, right);

	return is_part_number ? atoi(schematic[line_nr].data() + num_pos.first) : 0;
}


export void day3_1() {
	const auto schematic = ReadSchematic();

	int sum = 0;
	for (auto [line_nr, line] : schematic | views::enumerate) {
		size_t pos = 0;
		while (pos < line.length()) {
			const auto num_pos = GetNextNumber(line, pos);
			pos = num_pos.last + 1;

			if (num_pos.last < ssize(line)) {
				sum += GetPartNumber(schematic, line_nr, num_pos);
			}
		}
	}

	println("Day 3a: {}", sum);
}




map<pair<int, int>, vector<int>> gears;


static void UpdateGears(const vector<string>& schematic, int64_t line_nr, NumberPos num_pos) {
	const auto left = max(0, static_cast<int>(num_pos.first - 1));
	const auto right = static_cast<int>(min(num_pos.last + 1, ssize(schematic[line_nr]) - 1));

	const auto from_line = max(0, static_cast<int>(line_nr - 1));
	const auto to_line = static_cast<int>(min(line_nr + 1, ssize(schematic) - 1));

	for (int ln = from_line; ln <= to_line; ++ln) {
		for (int i = left; i <= right; ++i) {
			if (schematic[ln][i] == '*') {
				gears[{ln, i}].emplace_back(atoi(schematic[line_nr].data() + num_pos.first));
			}
		}
	}
}


export void day3_2() {
	const auto schematic = ReadSchematic();

	for (auto [line_nr, line] : schematic | views::enumerate) {
		size_t pos = 0;
		while (pos < line.length()) {
			const auto num_pos = GetNextNumber(line, pos);
			pos = num_pos.last + 1;

			if (num_pos.last < ssize(line)) {
				UpdateGears(schematic, line_nr, num_pos);
			}
		}
	}

	int sum = 0;
	for (const auto& [pos, nums] : gears) {
		if (nums.size() == 2) {
			sum += nums[0] * nums[1];
		}
	}

	println("Day 3b: {}", sum);
}
