#pragma once
#include <thc.h>
class ChessPlayer
{
public:

	virtual thc::Move MakeMove(thc::ChessRules& position) = 0 ;

private:
	
	

};

