#pragma once
#include "ChessPlayer.h"
class HumanPlayer final : public ChessPlayer
{
public:
	HumanPlayer();
	~HumanPlayer();


	virtual thc::Move MakeMove(thc::ChessRules& position) override;
};

