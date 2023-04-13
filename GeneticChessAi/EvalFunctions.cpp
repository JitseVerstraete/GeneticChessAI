#include "EvalFunctions.h"

const std::map<char, float> MaterialEval::pieceValues = { {'p', 1}, { 'b', 3 }, { 'n', 3 }, { 'r', 5 }, { 'q', 9 }, { 'k', 1'000'000 } };

float MaterialEval::operator()(const thc::ChessRules& position)
{
	float total{};

	for (int i{}; i < 64; ++i)
	{

		char piece = position.squares[i];
		if (piece == ' ') continue;

		bool white = isupper(piece);
		float value = pieceValues.find(tolower(piece))->second;

		total += white ? value : -value;
	}

	return total;
}
