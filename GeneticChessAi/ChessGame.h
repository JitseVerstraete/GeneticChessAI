#pragma once

#include "ChessPlayer.h"
#include "ChessGUI.h"


enum class GameResult
{
	Draw,
	WhiteWin,
	BlackWin,
	NoResult
};

class ChessGame final
{
public:
	ChessGame(ChessPlayer* white, ChessPlayer* black, bool drawDebug = false);
	~ChessGame();

	void PlayGame();
	bool GameDone();

	void setStartingPos(std::string FEN);

	thc::TERMINAL GetTerminalState() { return m_TerminalState; };
	thc::DRAWTYPE GetDrawState() { return m_DrawState; };
	GameResult GetGameResult();

	std::string GameStateDebugStr();

	std::string GetMovesRecord();


private:

	ChessPlayer* m_pWhite;
	ChessPlayer* m_pBlack;

	thc::ChessRules m_pPosition;

	thc::TERMINAL m_TerminalState;
	thc::DRAWTYPE m_DrawState;

	bool m_GameDone;

	std::vector<std::string> m_MovesRecord{};

	bool m_drawDebug{ false };

};

