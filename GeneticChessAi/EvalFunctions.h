#pragma once

#include <thc.h>
#include <map>

class MaterialEval final
{
public:
	float operator()(const thc::ChessRules& position);

	static const std::map<char, float> pieceValues; 
private:
};