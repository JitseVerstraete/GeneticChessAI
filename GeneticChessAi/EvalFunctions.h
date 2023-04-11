#pragma once

#include <thc.h>
#include <map>

class MaterialEval final
{
public:
	float operator()(const thc::ChessRules& position);

private:
	const std::map<char, float> m_pieceValues{ {'p',1}, {'b', 3}, {'n', 3}, {'r', 5}, {'q', 9}, {'k', 1'000'000} };
};