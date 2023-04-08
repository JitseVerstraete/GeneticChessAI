#include "ChessGame.h"

#include "HumanPlayer.h"
#include <iostream>

ChessGame::ChessGame(ChessPlayer* white, ChessPlayer* black, bool drawDebug)
	:m_pPosition{}, 
	m_drawDebug{drawDebug},
	m_pWhite{ white },
	m_pBlack{ black }
{
}

ChessGame::~ChessGame()
{

}



thc::TERMINAL ChessGame::PlayGame()
{
	thc::TERMINAL terminal{ thc::NOT_TERMINAL };


	while (terminal == thc::NOT_TERMINAL)
	{
		if (m_pPosition.WhiteToPlay())
		{
			m_pPosition.PlayMove(m_pWhite->MakeMove(m_pPosition));
		}
		else
		{
			m_pPosition.PlayMove(m_pBlack->MakeMove(m_pPosition));
		}

		
		if(m_drawDebug)
		{
			std::cout << m_pPosition.ToDebugStr();
		}

		m_pPosition.Evaluate(terminal);

	}

	return terminal;
}


