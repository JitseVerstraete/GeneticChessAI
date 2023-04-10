// GeneticChessAi.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <thc.h>
#include "ChessGUI.h"
#include "ChessGame.h"
#include "HumanPlayer.h"
#include "RandomBot.h"




int main(int argc, char** argv)
{
	{

		//TEST INSUFICCIENT MATERIAL DRAW STATES

		thc::ChessRules pos{};
		pos.Forsyth("8/8/7k/8/8/2B5/2N5/4K3 w - - 2 46");
		std::cout << pos.ToDebugStr();

		thc::DRAWTYPE type{};
		std::cout << pos.IsDraw(true, true, type) << std::endl;
		std::cout << pos.IsDraw(true, false, type) << std::endl;
		std::cout << pos.IsDraw(false, true,  type) << std::endl;
		std::cout << pos.IsDraw(false, false,  type) << std::endl;
	}

	{

		thc::ChessRules pos{};
		pos.Forsyth("8/8/7K/8/8/2b5/2n5/4k3 w - - 2 46");
		std::cout << pos.ToDebugStr();

		thc::DRAWTYPE type{};
		std::cout << pos.IsDraw(true, true, type) << std::endl;
		std::cout << pos.IsDraw(true, false, type) << std::endl;
		std::cout << pos.IsDraw(false, true, type) << std::endl;
		std::cout << pos.IsDraw(false, false, type) << std::endl;

	}





	/*
	HumanPlayer human{};
	RandomBot bot{};

	ChessGame game{&human, &bot, true };

	thc::TERMINAL termination = game.PlayGame();

	switch (termination)
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
	*/


}

