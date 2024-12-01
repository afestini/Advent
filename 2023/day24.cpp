export module day24;

import std;

using namespace std;


namespace {
	template<typename T>
	struct Vec3 {
		T x, y, z;

		auto operator<=>(const Vec3&) const = default;
		Vec3 operator-() const { return {-x, -y, -z}; }
		Vec3 operator+(Vec3 v) const { return {x + v.x, y + v.y, z + v.z}; }
		Vec3 operator-(Vec3 v) const { return {x - v.x, y - v.y, z - v.z}; }
		Vec3 operator/(Vec3 v) const { return {x / v.x, y / v.y, z / v.z}; }
		T Dot(Vec3 v) const { return x * v.x + y * v.y + z * v.z; }
		T LenSq() const { return Dot(*this); }
		T Len() const { return sqrt(LenSq()); }
		Vec3 Norm() const { const auto len = Len(); return {x / len, y / len, z / len}; }
		Vec3 Cross(Vec3 v) const { return {y * v.z - v.y * z, z * v.x - v.z * x, x * v.y - v.x * y}; }
	};

	template<typename T>
	Vec3<T> operator*(T s, Vec3<T> v) { return {s * v.x, s * v.y, s * v.z}; }

	struct Hail { Vec3<double> pos, vel; };
}


static vector<Hail> ReadInput() {
	ifstream input("day24.txt");

	vector<Hail> hailstones;

	string line;
	while (getline(input, line)) {
		auto& hail = hailstones.emplace_back();
		array coords = {&hail.pos.x, &hail.pos.y, &hail.pos.z, &hail.vel.x, &hail.vel.y, &hail.vel.z};

		for (int idx = 0; const auto corner : views::split(line, '@')) {
			for (const auto coord : views::split(corner, ',')) {
				ispanstream(coord) >> *coords[idx++];
			}
		}
	}
	return hailstones;
}


export void day24_1() {
	const auto start_time = chrono::high_resolution_clock::now();

	const auto hailstones = ReadInput();
	const double a_min = 200000000000000;
	const double a_max = 400000000000000;

	int count = 0;
	for (auto [i, a] : hailstones | views::enumerate) {
		for (const auto& b : ranges::subrange(hailstones.begin() + i + 1, hailstones.end())) {
			const auto delta_p = b.pos - a.pos;

			const auto cross = a.vel.x * b.vel.y - a.vel.y * b.vel.x;
			if (cross == 0) continue; // Parallel

			const auto t = (delta_p.x * b.vel.y - delta_p.y * b.vel.x) / cross;
			if (t < 0) continue; // Past of a

			const auto s = (delta_p.x * a.vel.y - delta_p.y * a.vel.x) / cross;
			if (s < 0) continue; // Past of b

			const auto is = a.pos + t * a.vel;
			if (is.x >= a_min && is.y >= a_min && is.x <= a_max && is.y <= a_max)
				++count;
		}
	}

	const auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start_time);
	println("Day 24a: {} ({})", count, duration);
}


static auto Plane(const Hail& s1, const Hail& s2) {
	const auto dp = s1.pos - s2.pos;
	const auto N = dp.Cross(s1.vel - s2.vel);
	const auto P = dp.Dot(s1.vel.Cross(s2.vel));
	return pair{N, P};
}


export void day24_2() {
	// Nicely stolen from Quantris.. now to understand and clean it up
	// Skipped the tests, since any parallel or co-planar lines would have made it so much easier
	const auto start_time = chrono::high_resolution_clock::now();

	const auto hailstones = ReadInput();

	const auto& s1 = hailstones[0];
	const auto& s2 = hailstones[1];
	const auto& s3 = hailstones[2];

	const auto [A, a] = Plane(s1, s2);
	const auto [B, b] = Plane(s1, s3);
	const auto [C, c] = Plane(s2, s3);

	const auto t = A.Dot(B.Cross(C));

	auto w = (a * B.Cross(C) + b * C.Cross(A) + c * A.Cross(B));
	w = Vec3(round(w.x / t), round(w.y / t), round(w.z / t));

	const auto w1 = s1.vel - w;
	const auto w2 = s2.vel - w;
	const auto ww = w1.Cross(w2);

	const auto E = ww.Dot(s2.pos.Cross(w2));
	const auto F = ww.Dot(s1.pos.Cross(w1));
	const auto G = s1.pos.Dot(ww);
	const auto S = ww.LenSq();

	const auto rock = E*w1 + -F*w2 + G*ww;
	const auto solution = (rock.x + rock.y + rock.z) / S;

	const auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start_time);
	println("Day 24b: {} ({})", solution, duration);
}
