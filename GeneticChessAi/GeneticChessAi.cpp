// GeneticChessAi.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <thc.h>
#include "ChessGUI.h"
#include "ChessGame.h"
#include "HumanPlayer.h"
#include "RandomBot.h"
#include "EvalFunctions.h"
#include "MiniMaxPlayer.h"
#include <iomanip>
#include <numeric>




int main(int argc, char** argv)
{
	
	thc::ChessRules pos{};
	pos.Forsyth("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w - - 0 1");

	MaterialEval eval{};
	MiniMaxPlayer miniMax{ 4, eval };

	std::vector<long long> timings;
	std::vector<int> treeSizes;
	int nrSamples{ 10 };
	for (int i{}; i < nrSamples; ++i)
	{
		auto start = std::chrono::high_resolution_clock::now();
		miniMax.MakeMove(pos);
		auto end = std::chrono::high_resolution_clock::now();

		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

		timings.push_back(duration.count());
		treeSizes.push_back(miniMax.LeafNodeCount());

	}

	for (int i{}; i < nrSamples; ++i)
	{
		std::cout << "thinking time: " << std::setw(6) << timings[i] << "ms" << std::endl;
		

	}

	std::cout << std::endl;
	std::cout << "avg thinking time: " << std::setw(6)  << std::accumulate(timings.begin(), timings.end(),0) / timings.size() << "ms" << std::endl;
	std::cout << "positions evaluated: " << std::setw(30) << std::left << treeSizes[0] << std::endl;
	


	


	/*
		MaterialEval evalFunc{};
		MiniMaxPlayer botWhite{4, evalFunc};

		RandomBot botBlack{};

		ChessGame game{&botWhite, &botBlack, false };
		//game.setStartingPos("8/7k/1R6/RK6/8/8/8/8 w - - 0 1");

		game.PlayGame();

		switch (game.GetTerminalState())
		{
		case thc::TERMINAL_WCHECKMATE:
			std::cout << "BLACK WON\n";
			break;
		case thc::TERMINAL_WSTALEMATE:
			std::cout << "STALEMATE\n";
			break;
		case thc::TERMINAL_BCHECKMATE:
			std::cout << "WHITE WON\n";
			break;
		case thc::TERMINAL_BSTALEMATE:
			std::cout << "STALEMATE\n";
			break;
		default:
			break;
		}

		switch (game.GetDrawState())
		{
		case thc::DRAWTYPE_50MOVE:
			std::cout << "50 MOVE RULE DRAW\n";
			break;
		case thc::DRAWTYPE_INSUFFICIENT:
		case thc::DRAWTYPE_INSUFFICIENT_AUTO:
			std::cout << "INSUFICIENT MATERIAL DRAW\n";
			break;
		case thc::DRAWTYPE_REPITITION:
			std::cout << "REPITITION DRAW\n";
			break;
		default:
			break;
		}

			std::cout << game.GetMovesRecord();

	*/
}

