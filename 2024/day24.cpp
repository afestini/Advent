export module day24;

import std;

using namespace std;
using namespace std::chrono;


struct System {
	struct Gate {
		string name;
		string op_name;
		Gate* in1 = nullptr;
		Gate* in2 = nullptr;
		vector<Gate*> outs;
		char value = -1;

		void eval() {
			//if (out >= 0) return;
			if (!in1 || !in2) return; // Reached and x/y

			in1->eval();
			in2->eval();
			switch (op_name[0]) {
			case 'A': value = in1->value & in2->value; break;
			case 'O': value = in1->value | in2->value; break;
			case 'X': value = in1->value ^ in2->value; break;
			}
		}
	};

	uint64_t register_value(char letter) const {
		uint64_t value = 0;
		for (auto& [name, gate] : gates) {
			if (name[0] == letter && gate.value == 1) {
				const auto shift = (name[1] - '0') * 10 + (name[2] - '0');
				value |= (1ULL << shift);
			}
		}
		return value;
	}

	uint64_t run() const {
		ranges::for_each(outputs, &Gate::eval);
		return register_value('z');
	}

	set<Gate*> analyze(const uint64_t bit) {
		set<Gate*> wrong;
		const auto reg = format("{:02}", bit);
		auto& x = gates.find("x" + reg)->second;
		auto& y = gates.find("y" + reg)->second;
		auto& z = gates.find("z" + reg)->second;

		// Arbitrary rules determined by checking the failing bits
		if (x.outs[0]->op_name != "AND" || x.outs[0]->outs.empty()) { wrong.emplace(x.outs[0]); }
		else if (x.outs[0]->outs[0]->op_name != "OR") { wrong.emplace(x.outs[0]); }

		if (y.outs[0]->op_name != "AND" || y.outs[0]->outs.empty()) { wrong.emplace(y.outs[0]); }
		else if (y.outs[0]->outs[0]->op_name != "OR") { wrong.emplace(y.outs[0]); }

		if (x.outs[1]->op_name != "XOR" || x.outs[1]->outs.empty()) { wrong.emplace(x.outs[1]); }
		else if (x.outs[1]->outs[0]->op_name != "AND") { wrong.emplace(x.outs[1]); }
		else if (x.outs[1]->outs[1]->op_name != "XOR" || x.outs[1]->outs[1]->name[0] != 'z') {
			wrong.emplace(x.outs[1]->outs[1]);
		}

		if (y.outs[1]->op_name != "XOR" || y.outs[1]->outs.empty()) { wrong.emplace(y.outs[1]); }
		else if (y.outs[1]->outs[0]->op_name != "AND") { wrong.emplace(y.outs[1]); }
		else if (y.outs[1]->outs[1]->op_name != "XOR" || y.outs[1]->outs[1]->name[0] != 'z') {
			wrong.emplace(y.outs[1]->outs[1]);
		}

		if (z.op_name != "XOR") wrong.emplace(&z);
		if (!wrong.contains(&z)) {
			if (z.in1->op_name != "OR") wrong.emplace(z.in1);
			else if (z.in2->op_name != "XOR") wrong.emplace(z.in2);
		}
		return wrong;
	}

	unordered_map<string, Gate> gates;
	vector<Gate*> outputs;
};


static auto file_input() {
	ifstream input("day24.txt");

	System system;

	string line;
	while (getline(input, line)) {
		if (line.empty()) break;

		System::Gate* gate = nullptr;
		for (auto [idx, str] : views::split(line, ": "sv) | views::enumerate) {
			string name(str.begin(), str.end());
			if (idx == 0) {
				gate = &system.gates.try_emplace(name).first->second;
				gate->name = name;
			}
			else gate->value = (str[0] == '1');
		}
	}

	while (getline(input, line)) {
		System::Gate gate;

		for (auto [idx, str] : views::split(line, " "sv) | views::enumerate) {
			string name(str.begin(), str.end());
			switch (idx) {
			case 0: gate.in1 = &system.gates[name]; break;
			case 1: gate.op_name = name; break;
			case 2: gate.in2 = &system.gates[name]; break;
			case 4: gate.name = name; break;
			default: break;
			}
		}
		auto& g = system.gates[gate.name];
		gate.outs = g.outs;
		if (gate.in1->op_name > gate.in2->op_name) swap(gate.in1, gate.in2);
		g = gate;
		g.in1->outs.push_back(&g);
		g.in2->outs.push_back(&g);
		if (g.name[0] == 'z')
			system.outputs.emplace_back(&g);
	}

	for (auto& gate : system.gates | views::values)
		ranges::sort(gate.outs, {}, &System::Gate::op_name);

	ranges::sort(system.outputs, {}, &System::Gate::name);
	return system;
}


export void day24_1() {
	const auto start_time = high_resolution_clock::now();

	System system = file_input();

	const auto output = system.run();

	const auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start_time);
	println("Day 24a: {} ({})", output, duration);
}


export void day24_2() {
	const auto start_time = high_resolution_clock::now();

	System system = file_input();

	vector<string> swapped;

	// First and last are different... skip and pray
	for (uint64_t bit = 1; bit + 1 < system.outputs.size(); ++bit) {
		vector wrong(from_range, system.analyze(bit));

		if (wrong.size() == 2) {
			swap(wrong[0]->op_name, wrong[1]->op_name);
			swap(wrong[0]->in1, wrong[1]->in1);
			swap(wrong[0]->in2, wrong[1]->in2);

			swapped.emplace_back(wrong[0]->name);
			swapped.emplace_back(wrong[1]->name);
		}
	}

	ranges::sort(swapped);
	const auto result = swapped | views::join_with(',') | ranges::to<string>();

	const auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start_time);
	println("Day 24b: {} ({})", result, duration);
}
