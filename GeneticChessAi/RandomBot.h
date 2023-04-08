#pragma once
#include "ChessPlayer.h"
class RandomBot final : public ChessPlayer
{
public:
	RandomBot();
	~RandomBot() = default;


	virtual thc::Move MakeMove(thc::ChessRules& position) override;
};

