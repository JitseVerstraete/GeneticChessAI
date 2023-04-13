#pragma once

#include <vector>
#include <thc.h>


void OrderMoves(std::vector<thc::Move>& moves, const thc::ChessRules& pos);

float MoveValuePrediction(const thc::Move& move, const thc::ChessRules& pos);

