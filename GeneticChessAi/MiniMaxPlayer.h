#pragma once
#include "ChessPlayer.h"
#include <functional>
#include <thc.h>
#include <iostream>
#include "MoveOrdering.h"
#include "EvalFunctions.h"
#include "TranspositionTable.h"


struct MoveValue
{
	thc::Move move;
	float value;
};


template <typename Eval>
class MiniMaxPlayer final : public ChessPlayer
{
public:
	MiniMaxPlayer(int depth, const Eval& eval, size_t transpositionSize);
	~MiniMaxPlayer() = default;

	int LeafNodeCount() { return m_leafNodeCounter; }
	int TranspositionCount() { return m_TranspositionsFound; }
	virtual thc::Move MakeMove(thc::ChessRules& position) override;


private:
	int m_depth;
	Eval m_EvalFunction;
	MoveValue MiniMax(thc::ChessRules& position, int depth, float alpha, float beta, bool maximizingPlayer, uint64_t hash);

	TranspositionTable m_tt;

	int m_leafNodeCounter{};
	int m_TranspositionsFound{};

	const float m_MateScore = 1'000'000.f;

};


template <typename Eval>
MiniMaxPlayer<Eval>::MiniMaxPlayer(int depth, const Eval& eval, size_t transpositionSize)
	:m_depth{ depth },
	m_EvalFunction{ eval },
	m_tt{ transpositionSize }
{
	//depth has to be at least one
	m_depth = std::max(m_depth, 1);
}

template <typename Eval>
thc::Move MiniMaxPlayer<Eval>::MakeMove(thc::ChessRules& position)
{
	//m_tt.Clear();
	m_leafNodeCounter = 0;
	m_TranspositionsFound = 0;
	bool maximize = position.WhiteToPlay();
	thc::ChessRules positionCopy = position;

	uint64_t hash = positionCopy.Hash64Calculate();
	MoveValue bestMove = MiniMax(positionCopy, m_depth, -FLT_MAX, FLT_MAX, maximize, hash);
	return bestMove.move;
}



template<typename Eval>
MoveValue MiniMaxPlayer<Eval>::MiniMax(thc::ChessRules& position, int depth, float alpha, float beta, bool maximizingPlayer, uint64_t hash)
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

	//check the transposition table
	TTEntry entry = m_tt.GetEntryAtHash(hash);
	if (entry.valid && entry.key == hash && entry.searchDepth >= depth)
	{

		//std::cout << "found an exact value node\n";
		m_TranspositionsFound++;
		return MoveValue(entry.move, entry.value);
	}


	MoveValue bestMove{};
	std::vector<thc::Move> moves{};
	position.GenLegalMoveList(moves);

	OrderMoves(moves, position);

	if (maximizingPlayer)
	{
		bestMove.value = -FLT_MAX;
		bestMove.move = thc::Move();

		for (thc::Move move : moves)
		{
			uint64_t updatedHash = position.Hash64Update(hash, move);
			position.PlayMove(move);
			assert(updatedHash == position.Hash64Calculate() && "the updated hash match the calculated one");
			float score = MiniMax(position, depth - 1, alpha, beta, false, updatedHash).value;
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
			uint64_t updatedHash = position.Hash64Update(hash, move);
			position.PlayMove(move);
			assert(updatedHash == position.Hash64Calculate() && "the updated hash match the calculated one");
			float score = MiniMax(position, depth - 1, alpha, beta, true, updatedHash).value;
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

	m_tt.StoreEvaluation(hash, bestMove.value, bestMove.move, depth);


	return bestMove;
}

