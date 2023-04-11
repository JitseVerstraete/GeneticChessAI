#pragma once
#include "ChessPlayer.h"
#include <functional>
#include <thc.h>
#include <iostream>

template <typename Eval>
class MiniMaxPlayer final : public ChessPlayer
{
public:
	MiniMaxPlayer(int depth, const Eval& eval);
	~MiniMaxPlayer() = default;


	virtual thc::Move MakeMove(thc::ChessRules& position) override;


private:
	int m_depth;
	Eval m_EvalFunction;
	float MiniMax(thc::ChessRules& position, int depth, bool maximizingPlayer);

};




template <typename Eval>
MiniMaxPlayer<Eval>::MiniMaxPlayer(int depth, const Eval& eval)
	:m_depth{ depth },
	m_EvalFunction{eval}
{
	//depth has to be at least one
	m_depth = std::max(m_depth, 1);
}

template <typename Eval>
thc::Move MiniMaxPlayer<Eval>::MakeMove(thc::ChessRules& position)
{
	bool maximize = position.WhiteToPlay();
	thc::ChessRules positionCopy = position;


	float bestValue{};
	thc::Move bestMove{};

	std::vector<thc::Move> moves{};
	positionCopy.GenLegalMoveList(moves);

	float depth = m_depth;




	if (maximize)
	{
		bestValue = -FLT_MAX;

		for (thc::Move move : moves)
		{
			positionCopy.PushMove(move);
			float score = MiniMax(positionCopy, depth - 1, false);
			positionCopy.PopMove(move);
			if (score > bestValue)
			{
				bestValue = score;
				bestMove = move;
			}
		}
	}
	else
	{
		bestValue = FLT_MAX;
		for (thc::Move move : moves)
		{
			positionCopy.PushMove(move);
			float score = MiniMax(positionCopy, depth - 1, true);
			positionCopy.PopMove(move);
			if (score < bestValue)
			{
				bestValue = score;
				bestMove = move;
			}
		}
	}

	return bestMove;
}



template<typename Eval>
float MiniMaxPlayer<Eval>::MiniMax(thc::ChessRules& position, int depth, bool maximizingPlayer)
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
		return 0.f;
	}

	//check for wins
	switch (terminal)
	{
	case thc::TERMINAL_WCHECKMATE:
		return -FLT_MAX;
		break;
	case thc::TERMINAL_BCHECKMATE:
		return FLT_MAX;
		break;
	default:
		break;
	}


	//if search depth is reached, return evaluation function result
	if (depth <= 0)
	{
		return m_EvalFunction(position);
	}


	float value{};
	std::vector<thc::Move> moves{};
	position.GenLegalMoveList(moves);

	if (maximizingPlayer)
	{
		value = -FLT_MAX;
		
		for (thc::Move move : moves)
		{
			position.PushMove(move);
			float score = MiniMax(position, depth - 1, false);
			position.PopMove(move);
			value = std::max(value, score);
		}
	}
	else
	{
		value = FLT_MAX;
		for (thc::Move move : moves)
		{
			position.PushMove(move);
			float score = MiniMax(position, depth - 1, true);
			position.PopMove(move);
			value = std::min(value, score);
		}
	}

	return value;
}

