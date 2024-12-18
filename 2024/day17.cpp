export module day17;

import std;

using namespace std;
using namespace std::chrono;


struct CPU {
	uint64_t reg_a;
	uint64_t reg_b;
	uint64_t reg_c;

	vector<uint8_t> program;
	vector<uint8_t> output;
	size_t instr_ptr = 0;

	void reset() {
		output.clear();
		instr_ptr = 0;
	}

	void execute(uint8_t op, uint8_t operand) {
		uint64_t combo_value = operand;
		switch (operand) {
		case 4: combo_value = reg_a; break;
		case 5: combo_value = reg_b; break;
		case 6: combo_value = reg_c; break;
		}

		switch (op) {
		case 0: // adv
			reg_a >>= combo_value; break;
		case 1: // bxl
			reg_b ^= operand; break;
		case 2: // bst
			reg_b = combo_value & 0b111; break;
		case 3: // jnz
			if (reg_a) instr_ptr = operand; break;
			break;
		case 4: // bxc
			reg_b ^= reg_c; break;
		case 5: // out
			output.emplace_back(static_cast<uint8_t>(combo_value & 0b111u)); break;
		case 6: // bdv
			reg_b = reg_a >> combo_value; break;
		case 7: // cdv
			reg_c = reg_a >> combo_value; break;
		default:
			println("Oh Oh"); break;
		}
	}

	void run() {
		while (instr_ptr < program.size()) {
			uint8_t op = program[instr_ptr++];
			uint8_t operand = program[instr_ptr++];
			execute(op, operand);
		}
	}
};


static CPU file_input() {
	ifstream input("day17.txt");

	CPU cpu;

	string line;
	while (getline(input, line)) {
		if (line.starts_with("Register A: "))
			from_chars(line.c_str() + 12, to_address(line.end()), cpu.reg_a);
		else if (line.starts_with("Register B: "))
			from_chars(line.c_str() + 12, to_address(line.end()), cpu.reg_b);
		else if (line.starts_with("Register C: "))
			from_chars(line.c_str() + 12, to_address(line.end()), cpu.reg_c);
		else if (line.starts_with("Program: ")) {
			for (auto num : line.substr(9) | views::split(',')) {
				string_view n(num);
				from_chars(n.data(), to_address(n.end()), cpu.program.emplace_back());
			}
		}
	}
	return cpu;
}


export void day17_1() {
	const auto start_time = high_resolution_clock::now();

	auto cpu = file_input();
	cpu.run();

	const auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start_time);
	println("Day 17a: {} ({})", cpu.output, duration);
}


static uint64_t fix_reg_a(CPU& cpu, uint64_t reg_a, size_t pos) {
	for (uint8_t bits = 0; bits < 8; bits++) {
		const auto tmp_reg_a = reg_a | bits;
		cpu.reset();
		cpu.reg_a = tmp_reg_a;
		cpu.run();
		
		if (cpu.output.front() == *(cpu.program.rbegin() + pos)) {
			if (pos + 1 >= cpu.program.size()) return tmp_reg_a;

			const auto next_reg_a = fix_reg_a(cpu, tmp_reg_a << 3, pos + 1);
			if (next_reg_a > 0) return next_reg_a;
		}
	}
	return 0;
}


export void day17_2() {
	const auto start_time = high_resolution_clock::now();

	auto cpu = file_input();
	const uint64_t a = fix_reg_a(cpu, 0, 0);

	const auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start_time);
	println("Day 17b: {} ({})", a, duration);
}
