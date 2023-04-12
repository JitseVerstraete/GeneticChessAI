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




int main(int argc, char** argv)
{
	MaterialEval evalFunc{};
	MiniMaxPlayer<MaterialEval> botWhite{5, evalFunc};

	RandomBot botBlack{};

	ChessGame game{&botWhite, &botBlack, true };
	game.setStartingPos("8/7k/1R6/RK6/8/8/8/8 w - - 0 1");

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




}

