#include "ChessMatch.h"

MatchResults ChessMatch::PlayMatch(ChessPlayer* p1, ChessPlayer* p2, unsigned int nrGames)
{
	MatchResults matchResults{};
	matchResults.p1 = p1;
	matchResults.p2 = p2;

	//create the matchups
	std::vector<std::pair<ChessPlayer*, ChessPlayer*>> pairings{};

	for (int i{}; i < nrGames; i++)
	{
		if (i < nrGames / 2)
		{
			pairings.push_back({ p1, p2 });
		}
		else
		{
			pairings.push_back({ p2, p1 });
		}
	}


	//play the games
	for (auto& pairing : pairings)
	{
		ChessGame game{ pairing.first, pairing.second };
		game.PlayGame();
		GameResult result = game.GetGameResult();
		
		switch (result)
		{
		case GameResult::Draw:
			matchResults.draws++;
			break;
		case GameResult::WhiteWin:
			
			break;
		case GameResult::BlackWin:
			break;
		case GameResult::NoResult:
			break;
		default:
			break;
		}

	}



	return matchResults;
}
