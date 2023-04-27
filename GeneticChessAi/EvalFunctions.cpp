#include "EvalFunctions.h"

//MaterialEval

const std::map<char, float> MaterialEval::pieceValues = { {'p', 1}, { 'b', 3 }, { 'n', 3 }, { 'r', 5 }, { 'q', 9 }, { 'k', 1'000'000 } };
float MaterialEval::operator()(const thc::ChessRules& position)
{
	float total{};

	for (int i{}; i < 64; ++i)
	{

		char piece = position.squares[i];
		if (piece == ' ') continue;

		bool white = isupper(piece);
		float value = pieceValues.find(tolower(piece))->second;

		total += white ? value : -value;
	}

	return total;
}



//NNEval

const std::map<char, int> NNEval::m_LayerIndices = {
	{'P', 0}, { 'N', 1 }, { 'B', 2 }, { 'R', 3 }, { 'Q', 4}, { 'K', 5 },
	{'p', 6}, { 'n', 7 }, { 'b', 8 }, { 'r', 9 }, { 'q', 10}, { 'k', 11 } };

NNEval::NNEval(NeuralNetwork* pNetwork)
{

	if (pNetwork->GetInputSize() != m_InputSize || pNetwork->GetOutputSize() != m_OutputSize)
	{
		throw "network input and outputs dont conform to the NNChess requirements";
	}

	m_pNetwork = pNetwork;
}

float NNEval::operator()(const thc::ChessRules& position)
{
	VectorXf inputVector = EncodeInput(position);
	float value = m_pNetwork->Calculate(inputVector)[0];

	return value;
}

VectorXf NNEval::EncodeInput(const thc::ChessRules& position)
{
	VectorXf inputVector{ m_InputSize };
	inputVector.setConstant(0.f);


	for (int squareIdx{}; squareIdx < 64; squareIdx++)
	{
		char piece = position.squares[squareIdx];
		auto it = m_LayerIndices.find(piece);

		if (it == m_LayerIndices.end()) continue;

		int layerIdx = it->second * 64;
		int index = layerIdx + squareIdx;

		inputVector[index] = islower(piece) ? -1 : 1;
	}

	return inputVector;
}


