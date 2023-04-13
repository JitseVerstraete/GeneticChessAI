#pragma once

#include <vector>
#include <thc.h>


void OrderMoves(std::vector<thc::Move>& moves, thc::ChessRules& pos);

float MoveValuePrediction(thc::Move& move, thc::ChessRules& pos);

