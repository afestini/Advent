import std;
import day1;
import day2;
import day3;
import day4;
import day5;
import day6;
import day7;
import day8;
import day9;
import day10;
import day11;
import day12;
import day13;
import day14;
import day15;
import day16;
import day17;
import day18;
import day19;

using namespace std;


int main(int argc, char** args) {
	if (argc > 1 && args[1] == "-i"sv)
		day15_2(true);

	day1_1();
	day1_2();

	day2_1();
	day2_2();

	day3_1();
	day3_2();

	day4_1();
	day4_2();

	day5_1();
	day5_2();

	day6_1();
#ifdef NDEBUG
	day6_2();
#endif

	day7_1();
	day7_2();

	day8_1();
	day8_2();

	day9_1();
	day9_2();

	day10_1();
	day10_2();

	day11_1();
#ifdef NDEBUG
	day11_2();
#endif

	day12();

	day13_1();
	day13_2();

	day14_1();
#ifdef NDEBUG
	day14_2();
#endif

	day15_1();
	day15_2();

	day16_1();
	day16_2();

	day17_1();
	day17_2();

	day18_1();
	day18_2();

	day19_1();
	day19_2();

	return 0;
}
