export module day4;

import std;

using namespace std;


struct Card {
	vector<int> winning;
	vector<int> yours;
	int count = 1;
};


static void ExtractNumbers(const auto& input, vector<int>& nums) {
	for (const auto& number_str : views::split(input, ' ')) {
		auto str = string_view(number_str);
		if (str.empty()) continue;
		from_chars(str.data(), to_address(str.end()), nums.emplace_back());
	}
}


static void ParseCard(string_view line, vector<Card>& cards) {
	auto card_numbers = views::split(line, ':') | ranges::to<vector>();
	auto win_yours = views::split(card_numbers[1], '|') | ranges::to<vector>();

	auto& card = cards.emplace_back();
	ExtractNumbers(win_yours[0], card.winning);
	ExtractNumbers(win_yours[1], card.yours);
}


static vector<Card> ParseCards() {
	ifstream input("day4.txt");
	if (!input) return {};

	vector<Card> cards;
	string line;
	while (getline(input, line)) {
		ParseCard(line, cards);
	}

	return cards;
}


export void day4_1() {
	const auto cards = ParseCards();

	int sum = 0;
	for (const auto& card : cards) {
		const auto is_winning = [&card](int n) { return ranges::contains(card.winning, n); };
		const auto matches = ranges::count_if(card.yours, is_winning);
		if (matches) {
			sum += (1 << (matches - 1));
		}
	}

	println("Day 4a: {}", sum);
}


export void day4_2() {
	auto cards = ParseCards();

	int sum = 0;
	for (const auto& [idx, card] : cards | views::enumerate) {
		const auto is_winning = [&card](int n) { return ranges::contains(card.winning, n); };
		const auto matches = ranges::count_if(card.yours, is_winning);

		for (int i = 1; i <= matches; ++i) {
			cards[idx + i].count += card.count;
		}
		sum += card.count;
	}

	println("Day 4b: {}", sum);
}
