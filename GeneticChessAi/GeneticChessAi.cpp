// GeneticChessAi.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <thc.h>
#include "ChessGUI.h"
#include "ChessGame.h"
#include <SDL_events.h>
#include "HumanPlayer.h"
#include "RandomBot.h"




int main(int argc, char** argv)
{
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


}

