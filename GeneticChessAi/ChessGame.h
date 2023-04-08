#pragma once

#include "ChessPlayer.h"
#include "ChessGUI.h"

class ChessGame 
{
public:
	ChessGame(ChessPlayer* white, ChessPlayer* black, bool drawDebug = false);
	~ChessGame();

	

	thc::TERMINAL PlayGame();


protected:

	ChessPlayer* m_pWhite;
	ChessPlayer* m_pBlack;

	thc::ChessRules m_pPosition;

	bool m_drawDebug{ false };
	

};

