#include "MoveOrdering.h"

#include <algorithm>
#include "EvalFunctions.h"

void OrderMoves(std::vector<thc::Move>& moves, thc::ChessRules& pos)
{
	auto greater = [&pos](thc::Move& lhs, thc::Move& rhs) -> bool
	{
		return MoveValuePrediction(lhs, pos) > MoveValuePrediction(rhs, pos);
	};

	std::sort(moves.begin(), moves.end(), greater);
}

float MoveValuePrediction(thc::Move& move, thc::ChessRules& pos)
{
	const float checkValue{ 50.f };
	const float captureValue{ 10.f };
	//mconst float AttackValue{0.2f};



	float value{};
	bool whiteToMove{ pos.WhiteToPlay() };


	//checks 
	auto enemyKing = whiteToMove ? pos.bking_square : pos.wking_square;
	pos.PushMove(move);
	if (pos.AttackedPiece((thc::Square)enemyKing))
	{
		value += checkValue;
	}
	pos.PopMove(move);


	//give a bonus prediction to captures based on piece type
	if (isalpha(move.capture))
	{
		float capturedPieceValue = MaterialEval::pieceValues.find(tolower(move.capture))->second;
		float capturingPieceValue = MaterialEval::pieceValues.find(tolower(pos.squares[move.src]))->second;
		value += captureValue * capturedPieceValue - capturingPieceValue;
	}

	return value;
}

