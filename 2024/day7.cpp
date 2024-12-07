export module day7;

import std;

using namespace std;


struct Equation {
	int64_t value = 0;
	vector<int64_t> numbers;
	bool valid = false;
};


static vector<Equation> file_input() {
	ifstream input("day7.txt");
	string line;
	vector<Equation> equations;
	while (getline(input, line)) {
		auto& eq = equations.emplace_back();

		string_view line_sv(line);
		const auto colon = line_sv.find(':');

		string_view v = line_sv.substr(0, colon);
		from_chars(to_address(v.begin()), to_address(v.end()), eq.value);

		for (auto num : line_sv | views::drop(colon + 2) | views::split(' ')) {
			from_chars(to_address(num.begin()), to_address(num.end()), eq.numbers.emplace_back());
		}
	}
	return equations;
}


static int digit_count(int64_t n) {
	int count = 1;
	while (n /= 10) ++count;
	return count;
}


#define OTHERS_ARE_SMARTER_THAN_ME 1

#if OTHERS_ARE_SMARTER_THAN_ME


static bool check(int64_t value, span<const int64_t> numbers, bool concat) {
	if (numbers.size() == 1) return value == numbers.back();

	const auto drop_last_nr = numbers.first(numbers.size() - 1);

	if ((value % numbers.back() == 0) && check(value / numbers.back(), drop_last_nr, concat))
		return true;

	if ((value > numbers.back() && check(value - numbers.back(), drop_last_nr, concat)))
		return true;

	if (!concat) return false;

	const auto pow10 = static_cast<int64_t>(pow(10, digit_count(numbers.back())));
	return ((value % pow10) == numbers.back()) && check(value / pow10, drop_last_nr, concat);
}


static int64_t sum_valid_equations(bool concat) {
	auto equations = file_input();

	atomic<int64_t> sum;
	for_each(execution::par_unseq, equations.begin(), equations.end(), [&sum, concat](auto& eq) {
		if (check(eq.value, eq.numbers, concat)) sum += eq.value;
	});
	return sum;
}

export void day7_1() {
	const auto start = chrono::high_resolution_clock::now();

	const auto sum = sum_valid_equations(false);

	const auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
	println("Day 7a: {} ({})", sum, duration);
}


export void day7_2() {
	const auto start = chrono::high_resolution_clock::now();

	const auto sum = sum_valid_equations(true);

	const auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
	println("Day 7b: {} ({})", sum, duration);
}

#else

static const array<function<uint64_t(uint64_t, uint64_t)>, 3> ops = {
	[](uint64_t a, uint64_t b) { return a + b; },
	[](uint64_t a, uint64_t b) { return a * b; },
	[](uint64_t a, uint64_t b) { return a * static_cast<uint64_t>(pow(10, digit_count(b))) + b; }
};


template<int N>
struct Base {
	explicit Base(size_t size) : digits(size) {}

	Base& operator++() {
		for (auto& digit : digits) {
			digit = (digit + 1) % N;
			overflow = (digit == 0);
			if (!overflow) break;
		}
		return *this;
	}

	vector<uint8_t> digits;
	bool overflow = false;
};


template<int OpCount>
static uint64_t validate(vector<Equation>&& equations) {
	for_each(execution::par_unseq, equations.begin(), equations.end(), [](auto& eq) {
		const auto num_ops = eq.numbers.size() - 1;

		for (Base<OpCount> combination(num_ops); !combination.overflow; ++combination) {
			auto result = eq.numbers[0];

			for (int i = 0; i < num_ops; ++i) {
				const int op_idx = combination.digits[i];
				result = ops[op_idx](result, eq.numbers[i + 1]);
				if (result > eq.value) break; // Numbers can only grow, abort
			}

			eq.valid = (result == eq.value);
			if (eq.valid) break;
		}
	});

	auto valid = equations | views::filter(&Equation::valid) | views::transform(&Equation::value);
	return reduce(valid.begin(), valid.end());
}


export void day7_1() {
	const auto start = chrono::high_resolution_clock::now();

	const auto sum = validate<2>(file_input());

	const auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
	println("Day 7a: {} ({})", sum, duration);
}


export void day7_2() {
	const auto start = chrono::high_resolution_clock::now();

	const auto sum = validate<3>(file_input());

	const auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
	println("Day 7b: {} ({})", sum, duration);
}

#endif
