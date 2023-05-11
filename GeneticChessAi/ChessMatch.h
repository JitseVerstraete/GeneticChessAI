#pragma once
#include "ChessPlayer.h"
#include "ChessGame.h"

struct MatchResults
{
	ChessPlayer* p1;
	ChessPlayer* p2;

	int p1wins;
	int draws;
	int p2wins;
};



class ChessMatch
{
public:
	
	//half of the matches will be played with p1 == white, the other half with p2 == white.
	static MatchResults PlayMatch(ChessPlayer* p1, ChessPlayer* p2, unsigned int nrGames);

private:
	struct GameRecord
	{
		ChessPlayer* pWhite;
		ChessPlayer* pBlack;
		GameResult result;
	};


};

