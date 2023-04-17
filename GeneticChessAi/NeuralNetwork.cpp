#include "NeuralNetwork.h"

#include <iostream>

NeuralNetwork::NeuralNetwork(std::initializer_list<int> layerSizes, float value)
{
	for (auto it = layerSizes.begin(); it + 1 != layerSizes.end(); ++it)
	{
		MatrixXf m{ *(it + 1), *it };
		m.setConstant(value);
		m_Layers.emplace_back(std::move(m));
	}


	assert(m_Layers.size() > 1 &&  "the neural network needs to be 2 layers big (one input & one output)");

}

void NeuralNetwork::PrintMatrices() const
{
	for (int i{}; i < m_Layers.size(); ++i)
	{
		std::cout << "Layer " << i << ": " << "in(" << m_Layers[i].cols() << ")" << "out(" << m_Layers[i].rows() << ")" << std::endl;
		std::cout << m_Layers[i] << std::endl << std::endl;
	}

}

int NeuralNetwork::GetInputSize() const
{
	return m_Layers[0].cols();
}

int NeuralNetwork::GetOutputSize() const
{
	return m_Layers[m_Layers.size() - 1].cols();
}

VectorXf NeuralNetwork::Calculate(const VectorXf& input) const
{
	VectorXf tempVector{ input };

	for (const MatrixXf& matrix : m_Layers)
	{
		tempVector = matrix * tempVector;
		//TODO: add activation function
	}

	return tempVector;
}
