#pragma once
#include "ChessPlayer.h"
#include <functional>
#include <thc.h>
#include <iostream>
#include "MoveOrdering.h"
#include "EvalFunctions.h"

struct MoveValue
{
	thc::Move move;
	float value;
};


template <typename Eval>
class MiniMaxPlayer final : public ChessPlayer
{
public:
	MiniMaxPlayer(int depth, const Eval& eval);
	~MiniMaxPlayer() = default;

	int LeafNodeCount() { return m_leafNodeCounter; }
	virtual thc::Move MakeMove(thc::ChessRules& position) override;


private:
	int m_depth;
	Eval m_EvalFunction;
	MoveValue MiniMax(thc::ChessRules& position, int depth, float alpha, float beta, bool maximizingPlayer);


	int m_leafNodeCounter{};

	const float m_MateScore = 1'000'000.f;

};


template <typename Eval>
MiniMaxPlayer<Eval>::MiniMaxPlayer(int depth, const Eval& eval)
	:m_depth{ depth },
	m_EvalFunction{ eval }
{
	//depth has to be at least one
	m_depth = std::max(m_depth, 1);
}

template <typename Eval>
thc::Move MiniMaxPlayer<Eval>::MakeMove(thc::ChessRules& position)
{
	m_leafNodeCounter = 0;
	bool maximize = position.WhiteToPlay();
	thc::ChessRules positionCopy = position;

	MoveValue bestMove = MiniMax(positionCopy, m_depth, -FLT_MAX, FLT_MAX, maximize);

	return bestMove.move;
}



template<typename Eval>
MoveValue MiniMaxPlayer<Eval>::MiniMax(thc::ChessRules& position, int depth, float alpha, float beta, bool maximizingPlayer)
{
	thc::TERMINAL terminal{};
	thc::DRAWTYPE draw{};
	position.Evaluate(terminal);
	position.IsDraw(false, false, draw);

	//check for draw or stalemate
	if (draw != thc::NOT_DRAW ||
		terminal == thc::TERMINAL_BSTALEMATE ||
		terminal == thc::TERMINAL_WSTALEMATE)
	{
		m_leafNodeCounter++;
		return MoveValue(thc::Move(), 0);
	}

	//check for wins (adding the remaining depth favors faster mates)
	switch (terminal)
	{
	case thc::TERMINAL_WCHECKMATE:
		m_leafNodeCounter++;
		return MoveValue(thc::Move(), -(m_MateScore + float(depth)));
		break;
	case thc::TERMINAL_BCHECKMATE:
		m_leafNodeCounter++;
		return MoveValue(thc::Move(), m_MateScore + float(depth));
		break;
	default:
		break;
	}


	//if search depth is reached, return evaluation function result
	if (depth <= 0)
	{
		m_leafNodeCounter++;
		return MoveValue(thc::Move(), m_EvalFunction(position));
	}


	MoveValue bestMove{};
	std::vector<thc::Move> moves{};
	//std::vector<bool> checks{};
	//std::vector<bool> mates{};
	//std::vector<bool> stalemates{};
	position.GenLegalMoveList(moves);

	OrderMoves(moves, position);



	if (maximizingPlayer)
	{
		bestMove.value = -FLT_MAX;
		bestMove.move = thc::Move();

		for (thc::Move move : moves)
		{
			position.PlayMove(move);
			float score = MiniMax(position, depth - 1, alpha, beta, false).value;
			position.UnplayMove(move);
			if (score > bestMove.value)
			{
				bestMove.value = score;
				bestMove.move = move;
			}
			alpha = std::max(alpha, bestMove.value);
			if (bestMove.value >= beta)
			{
				break;
			}

		}
	}
	else //minimizing
	{
		bestMove.value = FLT_MAX;
		bestMove.move = thc::Move();

		for (thc::Move move : moves)
		{
			position.PlayMove(move);
			float score = MiniMax(position, depth - 1, alpha, beta, true).value;
			position.UnplayMove(move);
			if (score < bestMove.value)
			{
				bestMove.value = score;
				bestMove.move = move;
			}
			beta = std::min(beta, bestMove.value);
			if (bestMove.value <= alpha)
			{
				break;
			}
		}
	}

	return bestMove;
}

