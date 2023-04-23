#pragma once

#include <thc.h>
#include <map>
#include "NeuralNetwork.h"

class MaterialEval final
{
public:
	float operator()(const thc::ChessRules& position);

	static const std::map<char, float> pieceValues;
private:
};



class NNEval
{
public:

	NNEval(NeuralNetwork* pNetwork);

	float operator()(const thc::ChessRules& position);

	VectorXf EncodeInput(const thc::ChessRules& pos);

private:
	//no ownership
	NeuralNetwork* m_pNetwork{ nullptr };


	static const std::map<char, int> m_LayerIndices;
	static const int m_InputSize = 768; //12 * 8 * 8 (12 pieces, 64 squares
	static const int m_OutputSize = 1;
};

