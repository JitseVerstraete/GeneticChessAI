#include "MoveOrdering.h"

#include <algorithm>

void OrderMoves(std::vector<thc::Move>& moves, thc::ChessRules& pos)
{
	auto greater = [&pos](const thc::Move& lhs, const thc::Move& rhs) -> bool
	{
		return MoveValuePrediction(lhs, pos) > MoveValuePrediction(rhs, pos);
	};

	std::sort(moves.begin(), moves.end(), greater);
}

float MoveValuePrediction(const thc::Move& move, thc::ChessRules& pos)
{

	const float checkValue{};
	const float captureValue{};
	const float AttackValue{};



	float value{};
	bool whiteToMove{ pos.WhiteToPlay() };

	//checks 
	char enemyKing = whiteToMove ? 'k' : 'K';

	for (int i{}; i < 64; ++i)
	{
		if (pos.squares[i] != enemyKing) continue;

		if (pos.AttackedSquare(thc::Square(pos.squares[i]), whiteToMove))
		{
			value += checkValue;
		}

	}

	//captures

	//attacks



	return value;
}

