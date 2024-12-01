export module day10;

import std;

using namespace std;


static string ReadMap() {
	ifstream input("day10.txt");
	if (!input) return {};

	string tiles;
	tiles.resize(filesystem::file_size("day10.txt"));
	const auto size = input.read(tiles.data(), tiles.size()).gcount();

	// Windows line endings /n/r count to filesize, but get filtered on read, trim the string
	// We could read line by line and get rid of the /n, but who needs all that copying?
	tiles.resize(size);

	return tiles;
}


class PipeTracker {
public:
	PipeTracker() {
		tiles = ReadMap();
		stride = tiles.find('\n') + 1;
		start = tiles.find('S');
		position = start;

		pipe_connections = {
			{'|', {-stride, stride}},
			{'-', {-1, 1}},
			{'L', {-stride, 1}},
			{'J', {-stride, -1}},
			{'7', {stride, -1}},
			{'F', {stride, 1}}
		};
	}

	int64_t Search() {
		StartDirections();
		FollowPipe();
		return steps / 2;
	}


	// Point in concave polygon approach, go in one direction and count intersections, odd -> inside
	uint64_t CountEnclosed() {
		uint64_t count = 0;

		// Search bottom up and cache results
		vector<int8_t> in_out(tiles.size());

		for (auto i = ssize(tiles) - 1; i >= 0; --i) {
			if ((tiles[i] & 0x80) || tiles[i] == '\n') continue;

			// We could check the shortest distance, but only checking down excludes a lot of cases.
			// We handle moving on top of an edge by remembering if the corner started from the left or right
			bool is_inside = false;
			char start_corner = 0;
			for (auto pos = i + stride; pos < ssize(tiles); pos += stride) {
				if (in_out[pos] == 1) { is_inside = !is_inside; break; }
				if (in_out[pos] == -1) break;

				if ((tiles[pos] & 0x80) == 0) continue; // Not part of the loop

#ifdef ORIGINAL
				const auto tile = static_cast<char>(tiles[pos] & 0x7F);
				if (tile == '|') continue; // Parallel, don't care

				if (tile == '-') { // Always an intersection
					is_inside = !is_inside;
				}
				else if ((tile == 'L' && start_corner == '7') || (tile == 'J' && start_corner == 'F')) {
					is_inside = !is_inside; // Intersect at bottom corner (really only two cases that can occur)
				}
				else if (tile == '7' || tile == 'F') { // Top corner, not an intersection yet, store for later
					start_corner = tile;
				}
#else
				// Had to watch William Y. Feng to realize the corners could be handled much easier
				if ("-7J"s.contains(tiles[pos] & 0x7F)) is_inside = !is_inside;
#endif
			}

			// Odd number of intersections means we started in side the polyg... pipe loop
			in_out[i] = is_inside ? 1 : -1;
			if (is_inside) ++count;
		}
		return count;
	}

private:
	void StartDirections() {
		vector<int64_t> start_dirs;
		for (auto dir : {-1LL, 1LL, -stride, stride}) {
			// Check out of bounds and if the pipe connects
			if (dir == -1      && ( start      % stride == 0   || !"-LF"s.contains(tiles[start + dir]))) continue;
			if (dir ==  1      && ((start + 1) % stride == 0   || !"-J7"s.contains(tiles[start + dir]))) continue;
			if (dir == -stride && (start < stride              || !"|7F"s.contains(tiles[start + dir]))) continue;
			if (dir ==  stride && (start + dir >= ssize(tiles) || !"|JL"s.contains(tiles[start + dir]))) continue;
			start_dirs.push_back(dir);
		}

		if (start_dirs.size() != 2) return; // If it's not, we have bigger problems

		// For part 2 only.. replace S with the actual pipe shape, so we don't have to worry about it later
		if      (start_dirs[0] == -1 && start_dirs[1] == 1)       tiles[start] = '-';
		else if (start_dirs[0] == -1 && start_dirs[1] == -stride) tiles[start] = 'J';
		else if (start_dirs[0] == -1 && start_dirs[1] ==  stride) tiles[start] = '7';
		else if (start_dirs[0] ==  1 && start_dirs[1] == -stride) tiles[start] = 'L';
		else if (start_dirs[0] ==  1 && start_dirs[1] ==  stride) tiles[start] = 'F';
		else                                                      tiles[start] = '|';

		Step(start_dirs[0]);
	}

	void Step(int64_t dir) {
		last_pos = position;
		tiles[last_pos] |= 0x80; // Mark all pipes in the loop by setting the high bit (not used for ASCII)
		position = position + dir;
		++steps;
	}

	void FollowPipe() {
		do {
			const auto dirs = pipe_connections[tiles[position]];
			const bool take_first = (position + dirs.first) != last_pos;
			Step(take_first ? dirs.first : dirs.second);
		} while (position != start);
	}

	string tiles;
	int64_t stride = 1;
	int64_t start = 0;
	int64_t last_pos = 0;
	int64_t position = 0;
	uint64_t steps = 0;
	unordered_map<char, pair<int64_t, int64_t>> pipe_connections;
};


export void day10_1() {
	const auto start = chrono::high_resolution_clock::now();

	const auto steps = PipeTracker().Search();

	const auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
	println("Day 10a: {} ({})", steps, duration);
}


export void day10_2() {
	const auto start = chrono::high_resolution_clock::now();

	PipeTracker tracker;
	tracker.Search();
	const auto sum = tracker.CountEnclosed();

	const auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
	println("Day 10b: {} ({})", sum, duration);
}
