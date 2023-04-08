// GeneticChessAi.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <thc.h>
#include "ChessGUI.h"
#include <SDL_events.h>




int main(int argc, char** argv)
{
	thc::ChessRules position{};
	position.Init();

	position.Forsyth("rnbqk1nr/ppp2ppp/8/2b1p2Q/4P3/2N5/PPPP2PP/R1B1KBNR b KQkq - 1 5");

	std::cout << position.ToDebugStr();
	
	ChessGUI gui{600};


	SDL_Event e{};
	while (!gui.IsQuit())
	{
		gui.HandleEvents();
		gui.DrawBoardState(&position);
		
	}


}

