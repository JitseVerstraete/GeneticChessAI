#include "NeuralNetwork.h"

#include <iostream>

NeuralNetwork::NeuralNetwork(std::initializer_list<int> layerSizes, std::function<float(float)> activationFunc, float value)
	:m_ActivationFunction{ activationFunc }
{

	//todo: add a column for biases in every matrix
	for (auto it = layerSizes.begin(); it + 1 != layerSizes.end(); ++it)
	{
		MatrixXf m{ *(it + 1), (*it) + 1 };
		m.setConstant(value);
		m_Layers.emplace_back(std::move(m));
	}


	if (m_Layers.size() <= 1)
	{
		throw "the neural network needs to be 2 layers big (one input & one output)";
	}
}

void NeuralNetwork::PrintMatrices() const
{
	for (int i{}; i < m_Layers.size(); ++i)
	{
		std::cout << "Layer " << i << ": " << "in(" << m_Layers[i].cols() << ")" << "out(" << m_Layers[i].rows() << ")" << std::endl;
		std::cout << m_Layers[i] << std::endl << std::endl;
	}
}

MatrixXf NeuralNetwork::GetLayerWeights(int layer)
{
	return MatrixXf();
}

VectorXf NeuralNetwork::GetLayerBiases(int layer)
{

	return VectorXf();
}

void NeuralNetwork::InitWeights(float value)
{
	for (auto& layer : m_Layers)
	{
		layer.leftCols(layer.cols() - 1).setConstant(value);
	}

}

void NeuralNetwork::InitWeights(std::function<float()> NullaryFunction)
{
	for (auto& layer : m_Layers)
	{
		auto block = layer.leftCols(layer.cols() - 1);
		block = block.NullaryExpr(block.rows(), block.cols(), NullaryFunction);
	}
}

void NeuralNetwork::InitBiases(float value)
{
	for (auto& layer : m_Layers)
	{
		layer.rightCols(1).setConstant(value);
	}
}

void NeuralNetwork::InitBiases(std::function<float()> NullaryFunction)
{
	for (auto& layer : m_Layers)
	{
		auto block = layer.rightCols(1);
		block = block.NullaryExpr(block.rows(), block.cols(), NullaryFunction);
	}
}

int NeuralNetwork::GetInputSize() const
{
	return m_Layers[0].cols() - 1;
}

int NeuralNetwork::GetOutputSize() const
{
	return m_Layers[m_Layers.size() - 1].rows();
}

VectorXf NeuralNetwork::Calculate(VectorXf input) const
{

	for (const MatrixXf& matrix : m_Layers)
	{
		//append a 1 to the input vector which will add 
		input.conservativeResize(input.size() + 1);
		input(input.size() - 1) = m_BiasInputConstant;

		//compute weight matrix
		input = matrix * input;

		//activation function
		input = input.unaryExpr(m_ActivationFunction);


	}

	return input;
}
