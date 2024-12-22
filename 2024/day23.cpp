export module day23;

import std;

using namespace std;
using namespace std::chrono;


using PC = array<char, 2>;

struct PCHash {
	size_t operator()(PC pc) const { return bit_cast<uint16_t>(pc); }
};


static auto file_input() {
	ifstream input("day23.txt");

	unordered_map<PC, set<PC>, PCHash> network;

	string line;
	while (getline(input, line)) {
		array<PC, 2> con{};
		for (auto [idx, name] : views::split(line, '-') | views::enumerate) {
			con[idx] = {name[0], name[1]};
		}
		network[con[0]].emplace(con[1]);
		network[con[1]].emplace(con[0]);
	}

	return network;
}


export void day23_1() {
	const auto start_time = high_resolution_clock::now();

	auto network = file_input();

	int triplets = 0;

	for (const auto& [pc1, cons1] : network) {
		for (const auto [i, pc2] : cons1 | views::enumerate) {
			if (pc1 > pc2) continue;

			const auto& cons2 = network[pc2];

			for (const auto pc3 : cons1 | views::drop(i + 1)) {
				if (cons2.contains(pc3)) {
					array triplet {pc1, pc2, pc3};
					if (ranges::any_of(triplet, [](const auto& name) { return name[0] == 't'; })) {
						++triplets;
					}
				}
			}
		}
	}

	const auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start_time);
	println("Day 23a: {} ({})", triplets, duration);
}


export void day23_2() {
	const auto start_time = high_resolution_clock::now();

	auto network = file_input();
	vector<PC> largest_group;

	for (const auto& [pc1, connections] : network) {
		vector group{pc1};

		for (const auto [i, pc2] : connections | views::enumerate) {
			const auto connected = [&cons = network[pc2]](const auto& pc) { return ranges::contains(cons, pc); };

			if (ranges::all_of(connections | views::drop(i + 1), connected))
				group.emplace_back(pc2);
		}

		if (group.size() > largest_group.size())
			largest_group = move(group);
	}

	ranges::sort(largest_group);
	const auto password = largest_group | views::join_with(',');

	const auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start_time);
	println("Day 23b: {:s} ({})", password, duration);
}
