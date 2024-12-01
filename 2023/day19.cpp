export module day19;

import std;

using namespace std;


struct Part {
	uint64_t x;
	uint64_t m;
	uint64_t a;
	uint64_t s;
};


struct Rule {
	vector<Rule>* dst = nullptr;
	uint64_t Part::* attr = nullptr;
	uint64_t ref = 0;
	char op = ' ';
};
using Workflow = vector<Rule>;


struct Input {
	using WorkflowTable = unordered_map<string, Workflow>;
	using PartList = vector<Part>;

	WorkflowTable workflows{
		{"A", {{.op = 'A'}}},
		{"R", {{.op = 'R'}}}
	};
	PartList parts;
	Workflow* in_flow = nullptr;
};


static Input ReadInput() {
	ifstream file("day19.txt");

	Input input;
	input.parts.reserve(200);
	input.workflows.reserve(600);

	string line;
	bool parse_rules = true;
	while (getline(file, line)) {
		string_view sv = line;

		if (sv.empty()) {
			parse_rules = false;
		}
		else if (parse_rules) {
			const auto name = sv.substr(0, sv.find('{'));
			auto& flow = input.workflows[string(name)];

			if (name == "in") input.in_flow = &flow;

			const string_view rules_line = sv.substr(name.size() + 1, sv.size() - name.size() - 2);

			for (const auto rule_str : rules_line | views::split(',')) {
				auto& rule = flow.emplace_back();
				const string_view check_str(rule_str);

				if (const auto colon_pos = check_str.find(':'); colon_pos != string::npos) {
					if (check_str[0] == 'x') rule.attr = &Part::x;
					else if (check_str[0] == 'm') rule.attr = &Part::m;
					else if (check_str[0] == 'a') rule.attr = &Part::a;
					else if (check_str[0] == 's') rule.attr = &Part::s;

					rule.op = check_str[1];

					from_chars(check_str.data() + 2, check_str.data() + colon_pos, rule.ref);

					const auto dst = check_str.substr(colon_pos + 1);
					rule.dst = &input.workflows[string(dst)];
				}
				else {
					rule.dst = &input.workflows[string(check_str)];
				}
			}
		}
		else {
			auto& part = input.parts.emplace_back();

			for (auto attr : sv.substr(1, sv.size() - 2) | views::split(',')) {
				uint64_t value;
				from_chars(attr.data() + 2, to_address(attr.end()), value);

				if (attr[0] == 'x') part.x = value;
				else if (attr[0] == 'm') part.m = value;
				else if (attr[0] == 'a') part.a = value;
				else if (attr[0] == 's') part.s = value;
			}
		}
	}
	return input;
}


export void day19_1() {
	const auto start = chrono::high_resolution_clock::now();

	const auto input = ReadInput();

	uint64_t sum = 0;

	for (const auto& part : input.parts) {
		for (const auto* wf = input.in_flow; wf;) {
			for (const auto& rule : *wf) {
				if ((rule.op == '<' && (part.*rule.attr) < rule.ref) ||
				    (rule.op == '>' && (part.*rule.attr) > rule.ref))
				{
					wf = rule.dst;
					break;
				}
				else {
					wf = rule.dst;

					if (rule.op == 'A')
						sum += part.x + part.s + part.m + part.a;
				}
			}
		}
	}

	const auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
	println("Day 19a: {} ({})", sum, duration);
}


struct PartRange {
	Part min{1, 1, 1, 1};
	Part max{4000, 4000, 4000, 4000};
};

static vector<PartRange> passing_ranges;


static void Check(const Workflow& wf, PartRange pr) {
	for (const auto& rule : wf) {
		if (rule.op == 'A') {
			passing_ranges.emplace_back(pr);
		}
		else if (rule.op == ' ') {
			Check(*rule.dst, pr);
		}
		else if (rule.op == '<') {
			auto new_range = pr;
			new_range.max.*rule.attr = rule.ref - 1;
			pr.min.*rule.attr = rule.ref;
			Check(*rule.dst, new_range);
		}
		if (rule.op == '>') {
			auto new_range = pr;
			new_range.min.*rule.attr = rule.ref + 1;
			pr.max.*rule.attr = rule.ref;
			Check(*rule.dst, new_range);
		}
	}
}


export void day19_2() {
	const auto start = chrono::high_resolution_clock::now();

	const auto input = ReadInput();
	Check(*input.in_flow, PartRange());

	uint64_t sum = 0;
	for (const auto& pr : passing_ranges) {
		sum += (pr.max.x - pr.min.x + 1) *
		       (pr.max.m - pr.min.m + 1) *
		       (pr.max.a - pr.min.a + 1) *
		       (pr.max.s - pr.min.s + 1);
	}

	const auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
	println("Day 19b: {} ({})", sum, duration);
}
