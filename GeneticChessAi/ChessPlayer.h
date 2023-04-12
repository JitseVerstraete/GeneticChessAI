#pragma once
#include <thc.h>
class ChessPlayer
{
public:

	ChessPlayer() = default;
	virtual ~ChessPlayer() = default;


	virtual thc::Move MakeMove(thc::ChessRules& position) = 0 ;

private:
	
	

};

