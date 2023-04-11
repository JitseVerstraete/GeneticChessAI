#include "EvalFunctions.h"

float MaterialEval::operator()(const thc::ChessRules& position)
{
	float total{};

	for (int i{}; i < 64; ++i)
	{

		char piece = position.squares[i];
		if (piece == ' ') continue;

		bool white = isupper(piece);
		float value = m_pieceValues.find(tolower(piece))->second;

		total += white ? value : -value;
	}

	return total;
}
