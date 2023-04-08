#include "RandomBot.h"
#include <time.h>

RandomBot::RandomBot()
{
    srand(time(NULL));
}

thc::Move RandomBot::MakeMove(thc::ChessRules& position)
{
    thc::Move move{};


    std::vector<thc::Move> moves{};
    position.GenLegalMoveList(moves);

    move = moves[rand() % moves.size()];


    return move;
}
