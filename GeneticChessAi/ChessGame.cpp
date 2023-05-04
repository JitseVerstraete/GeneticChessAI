#include "ChessGame.h"

#include "HumanPlayer.h"
#include "EvalFunctions.h"
#include <iostream>

ChessGame::ChessGame(ChessPlayer* white, ChessPlayer* black, bool drawDebug)
	: m_pWhite{ white },
	m_pBlack{ black },
	m_pPosition{},
	m_GameDone{ false },
	m_TerminalState{ thc::NOT_TERMINAL },
	m_DrawState{ thc::NOT_DRAW },
	m_drawDebug{ drawDebug }
{
}

ChessGame::~ChessGame()
{

}



void ChessGame::PlayGame()
{
	thc::TERMINAL terminal{ thc::NOT_TERMINAL };
	thc::DRAWTYPE draw{ thc::NOT_DRAW };

	thc::Move tempMove{};

	while (terminal == thc::NOT_TERMINAL && draw == thc::NOT_DRAW)
	{
		if (m_drawDebug)
		{
			std::cout << m_pPosition.ToDebugStr();
			
		}

		if (m_pPosition.WhiteToPlay())
		{
			tempMove = m_pWhite->MakeMove(m_pPosition);
		
		}
		else
		{
			tempMove = m_pBlack->MakeMove(m_pPosition);
		}

		m_MovesRecord.emplace_back(tempMove.NaturalOut(&m_pPosition));

		m_pPosition.PlayMove(tempMove);

		

		m_pPosition.Evaluate(terminal);
		m_pPosition.IsDraw(false, false, draw);
	}

	if (m_drawDebug)
	{
		std::cout << m_pPosition.ToDebugStr();
		
	}

	m_TerminalState = terminal;
	m_DrawState = draw;
}

bool ChessGame::GameDone()
{
	return (m_DrawState != thc::NOT_DRAW) || (m_TerminalState != thc::NOT_TERMINAL);
}

void ChessGame::setStartingPos(std::string fen)
{
	m_pPosition.Forsyth(fen.c_str());
}

GameResult ChessGame::GetGameResult()
{
	if (GetTerminalState() == thc::NOT_TERMINAL && GetDrawState() == thc::NOT_DRAW)
		return GameResult::NoResult;

	switch (GetTerminalState())
	{
	case thc::TERMINAL_WCHECKMATE:
		return GameResult::BlackWin;
		break;

	case thc::TERMINAL_BCHECKMATE:
		return GameResult::WhiteWin;
		break;

	case thc::TERMINAL_WSTALEMATE:
	case thc::TERMINAL_BSTALEMATE:
		return GameResult::Draw;
		break;
	default:
		break;
	}

	switch (GetDrawState())
	{
	case thc::DRAWTYPE_50MOVE:		
	case thc::DRAWTYPE_INSUFFICIENT:
	case thc::DRAWTYPE_INSUFFICIENT_AUTO:
	case thc::DRAWTYPE_REPITITION:
		return GameResult::Draw;
		break;
	
	default:
		break;
	}

	return GameResult::NoResult;
}

std::string ChessGame::GameStateDebugStr()
{
	return m_pPosition.ToDebugStr();
}

std::string ChessGame::GetMovesRecord()
{
	std::string gameString{};

	
	for (int i{}; i < m_MovesRecord.size(); ++i)
	{
		if (i % 2 == 0)
		{
			gameString.append(std::to_string(1 + int(i / 2)) + ". ");
		}

		gameString.append(m_MovesRecord[i] + " ");
	}


	return gameString;
}


