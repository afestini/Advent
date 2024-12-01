export module day22;

import std;

using namespace std;

struct Vec3 { int x, y, z; };

struct Brick {
	Vec3 min;
	Vec3 max;
	set<Brick*> supports;
	set<const Brick*> supported_by;
	bool falling = false;
};

struct Stack {
	vector<Brick> bricks;
	int width = 0;
	int height = 0;
};


static Stack ReadInput() {
	ifstream input("day22.txt");

	Stack stack;

	string line;
	while (getline(input, line)) {
		auto& brick = stack.bricks.emplace_back();
		array coords = {&brick.min.x, &brick.min.y, &brick.min.z, &brick.max.x, &brick.max.y, &brick.max.z};

		for (int idx = 0; const auto corner : views::split(line, '~')) {
			for (const auto coord : views::split(corner, ',')) {
				from_chars(coord.data(), to_address(coord.end()), *coords[idx++]);
			}
		}
		stack.width = max(stack.width, brick.max.x + 1);
		stack.height = max(stack.height, brick.max.y + 1);
	}
	return stack;
}


static void DropStack(Stack& stack) {
	struct Z_entry {
		int z = 0;
		Brick* brick = nullptr;
	};
	vector<Z_entry> z_map(stack.width * stack.height);

	ranges::sort(stack.bricks, {}, [](const auto& b) {return b.min.z; });

	// We don't want to search ourselves stupid, so link all supporters and supportees
	// We also keep a z buffer inspired grid of highest z and the corresponding bricks as we go
	for (auto& brick : stack.bricks) {
		int max_z = 0;
		for (int y = brick.min.y; y <= brick.max.y; ++y) {
			for (int x = brick.min.x; x <= brick.max.x; ++x) {
				max_z = max(max_z, z_map[y * stack.width + x].z);
			}
		}

		brick.max.z = max_z + brick.max.z - brick.min.z + 1;
		brick.min.z = max_z + 1;

		for (int y = brick.min.y; y <= brick.max.y; ++y) {
			for (int x = brick.min.x; x <= brick.max.x; ++x) {
				auto& z_entry = z_map[y * stack.width + x];
				if (z_entry.z == max_z && z_entry.brick) {
					z_entry.brick->supports.insert(&brick);
					brick.supported_by.insert(z_entry.brick);
				}
				z_entry.z = brick.max.z;
				z_entry.brick = &brick;
			}
		}
	}
}


export void day22_1() {
	const auto start = chrono::high_resolution_clock::now();

	auto stack = ReadInput();
	DropStack(stack);

	const auto count = ranges::count_if(stack.bricks, [](const auto& b) {
		return ranges::all_of(b.supports, [](const auto s) { return s->supported_by.size() != 1; });
	});

	const auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
	println("Day 22a: {} ({})", count, duration);
}


export void day22_2() {
	const auto start = chrono::high_resolution_clock::now();

	auto stack = ReadInput();
	DropStack(stack);

	int64_t sum = 0;
	queue<Brick*> to_check;

	for (auto& brick : stack.bricks) {
		for (auto& b : stack.bricks) b.falling = false;

		brick.falling = true;
		to_check.emplace(&brick);

		while (!to_check.empty()) {
			Brick* b = to_check.front();
			to_check.pop();

			for (auto supportee : b->supports) {
				if (supportee->falling) continue;

				if (ranges::all_of(supportee->supported_by, &Brick::falling)) {
					supportee->falling = true;
					to_check.push(supportee);
					++sum;
				}
			}
		}
	}

	const auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
	println("Day 22b: {} ({})", sum, duration);
}
