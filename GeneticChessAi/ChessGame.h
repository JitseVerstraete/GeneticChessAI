#pragma once

#include "ChessPlayer.h"
#include "ChessGUI.h"

class ChessGame final
{
public:
	ChessGame(ChessPlayer* white, ChessPlayer* black, bool drawDebug = false);
	~ChessGame();

	

	void PlayGame(thc::TERMINAL& terminalState, thc::DRAWTYPE& drawState);


private:

	ChessPlayer* m_pWhite;
	ChessPlayer* m_pBlack;

	thc::ChessRules m_pPosition;

	bool m_drawDebug{ false };
	

};

