export module day7;

import std;

using namespace std;


// Yes, globals.. boo.. fuck it
static bool use_jokers = false;


struct Hand {
	enum Type { High, OnePair, TwoPair, Three, FullHouse, Four, Five };

	static constexpr array card_list = {'2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A'};

	static int CardValue(char c) {
		if (c == 'J' && use_jokers) return -1;
		return static_cast<int>(distance(card_list.begin(), ranges::find(card_list, c)));
	}

	string cards;
	uint64_t bid;
	Type type = Type::High;

	strong_ordering operator<=>(const Hand& other) const {
		if (type != other.type) return type <=> other.type;

		for (int i = 0; i < 5; ++i) {
			if (cards[i] != other.cards[i]) {
				return CardValue(cards[i]) <=> CardValue(other.cards[i]);
			}
		}
		return strong_ordering::equivalent;
	}
};


static vector<Hand> ParseHands() {
	ifstream input("day7.txt", ios::binary);

	vector<Hand> hands;

	string data;
	data.resize(filesystem::file_size("day7.txt"));
	input.read(data.data(), data.size());

	ispanstream str(span(data.begin(), data.end()));
	do {
		auto& hand = hands.emplace_back();
		str >> hand.cards >> hand.bid;
	} while (str);

	return hands;
}


static void DetermineType(Hand& hand) {
	// Fill card counts (use a map to be lazy)
	unordered_map<char, int> counts;
	int jokers = 0;

	for (auto card : hand.cards) {
		if (card == 'J' && use_jokers) ++jokers;
		else counts[card]++;
	}

	// Convert to vector and sort by count (more inefficient laziness)
	auto vec = views::values(counts) | ranges::to<vector>();
	ranges::sort(vec, greater());

	if (vec.empty() || (vec[0] + jokers == 5)) hand.type = Hand::Five;
	else if (vec[0] + jokers == 4) hand.type = Hand::Four;
	else if (vec[0] + jokers == 3) hand.type = (vec[1] == 2) ? Hand::FullHouse : Hand::Three;
	else if (vec[0] + jokers == 2) hand.type = (vec[1] == 2) ? Hand::TwoPair : Hand::OnePair;
	else hand.type = Hand::High;
}


static uint64_t EvaluateHands(bool jokers) {
	use_jokers = jokers;

	auto hands = ParseHands();

	for_each(execution::par_unseq, hands.begin(), hands.end(), &DetermineType);
	sort(execution::par_unseq, hands.begin(), hands.end(), less());

	uint64_t sum = 0;
	for (const auto& [idx, hand] : hands | views::enumerate) {
		sum += hand.bid * (idx + 1);
	}

	return sum;
}


export void day7_1() {
	const auto start = chrono::high_resolution_clock::now();

	const auto result = EvaluateHands(false);

	const auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
	println("Day 7a: {} ({})", result, duration);
}


export void day7_2() {
	const auto start = chrono::high_resolution_clock::now();

	const auto result = EvaluateHands(true);

	const auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
	println("Day 7b: {} ({})", result, duration);
}
