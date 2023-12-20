export module day20;

import std;

using namespace std;

namespace {

struct Module;

struct Pulse {
	Module* receiver;
	bool state;
};


struct Module {
	vector<const Module*> inputs;
	vector<Module*> outputs;
	bool state = false;

	int64_t cycle = 0;
	bool went_high = false;

	// Seriously.. SCREW OO, wasted hours on workarounds to avoid multiple passes during setup or map lookups.
	// Module types are often unknown when referenced, but should already connect inputs.
	// Type must be changeable after the fact and is only defined by the pulse response, anyways.
	// Better to have it ugly in one spot (here) than all over the place!
	bool ProcessPulse(bool pulse) { return (this->*response_function)(pulse); }

	bool (Module::* response_function)(bool) = &Module::Output;

	bool Output(bool) { return false; }

	bool Broadcast(bool pulse) {
		state = pulse;
		return true;
	}

	bool FlipFlop(bool pulse) {
		if (pulse) return false;

		state = !state;
		return true;
	}

	bool Conjunction(bool) {
		state = !ranges::all_of(inputs, identity(), &Module::state);

		went_high = state;
		return true;
	}
};


struct Circuit {
	int64_t presses = 0;
	int64_t highs = 0;
	int64_t lows = 0;

	queue<Pulse> pulses;

	unordered_map<string, Module> modules;

	Module* broadcaster = nullptr;


	void SendPulse(Module& to, bool state) {
		state ? ++highs : ++lows;
		pulses.push({&to, state});
	}

	void PushButton() {
		++presses;
		SendPulse(*broadcaster, false);

		while (!pulses.empty()) {
			const auto [mod, state] = pulses.front();
			pulses.pop();

			if (mod->ProcessPulse(state)) {
				for (const auto output : mod->outputs) {
					SendPulse(*output, mod->state);
				}
			}
			if (!mod->cycle && mod->went_high) mod->cycle = presses;
		}
	}
};


Circuit ReadInput() {
	ifstream input("day20.txt");

	Circuit circuit;

	string line;
	while (getline(input, line)) {
		const auto mod_outputs = views::split(line, " -> "s) | ranges::to<vector>();

		auto name = string_view(mod_outputs[0]);
		char type = 'B';

		if (name != "broadcaster") {
			type = name[0];
			name.remove_prefix(1);
		}

		auto& mod = circuit.modules[string(name)];

		if (type == '%') mod.response_function = &Module::FlipFlop;
		else if (type == '&') mod.response_function = &Module::Conjunction;
		else if (type == 'B') mod.response_function = &Module::Broadcast;

		for (const auto& out : views::split(mod_outputs[1], ", "s)) {
			auto& output = circuit.modules[string(out.begin(), out.end())];

			output.inputs.emplace_back(&mod);
			mod.outputs.emplace_back(&output);
		}
	}

	circuit.broadcaster = &circuit.modules["broadcaster"];
	return circuit;
}

}


export void day20_1() {
	const auto start = chrono::high_resolution_clock::now();

	auto circuit = ReadInput();
	for (int i = 0; i < 1000; ++i) {
		circuit.PushButton();
	}

	const auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
	println("Day 20a: {} ({})", circuit.lows * circuit.highs, duration);
}


export void day20_2() {
	const auto start = chrono::high_resolution_clock::now();

	auto circuit = ReadInput();
	const auto watch_list = circuit.modules.at("rx").inputs[0]->inputs;

	while (!ranges::all_of(watch_list, identity(), &Module::cycle)) {
		circuit.PushButton();
	}

	const auto LCM = ranges::fold_left(watch_list, 1ULL, [](int64_t LCM, const auto m) {
		return lcm(LCM, m->cycle);
	});

	const auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
	println("Day 20b: {} ({})", LCM, duration);
}
