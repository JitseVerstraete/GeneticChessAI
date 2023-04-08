#include "HumanPlayer.h"

#include <iostream>

HumanPlayer::HumanPlayer()
{
}

HumanPlayer::~HumanPlayer()
{
}

thc::Move HumanPlayer::MakeMove(thc::ChessRules& position)
{

	thc::Move move{};
	std::string moveInput{};
	bool okay{ false };

	while(!okay)
	{
		std::cout << "\nENTER YOU MOVE (standard notation): ";
		std::cin >> moveInput;
		okay = move.NaturalIn(&position, moveInput.c_str());
	}



	return move;
}
