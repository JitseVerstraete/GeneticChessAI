#include "NeuralNetwork.h"

#include <iostream>

NeuralNetwork::NeuralNetwork(const std::vector<LayerSettings>& layers, float value)
{
	if (layers.size() <= 1)
	{
		throw "the neural network needs to be 2 layers big (one input & one output)";
	}

	for (auto it = layers.begin(); it + 1 != layers.end(); ++it)
	{
		MatrixXf matrix{ (it + 1)->nrNodes, it->nrNodes + 1 };
		matrix.setConstant(value);

		VectorXf inputVec(matrix.cols());
		inputVec.setConstant(0);
		inputVec(inputVec.rows() - 1) = m_BiasInputConstant;

		m_Layers.emplace_back(NNLayer{ std::move(matrix), std::move(inputVec) , (it + 1)->activationFunc });
	}

}

void NeuralNetwork::PrintMatrices() const
{
	for (int i{}; i < m_Layers.size(); ++i)
	{
		std::cout << "Layer " << i << ": " << "in(" << m_Layers[i].matrix.cols() << ")" << "out(" << m_Layers[i].matrix.rows() << ")" << std::endl;
		std::cout << m_Layers[i].matrix << std::endl << std::endl;
	}
}


void NeuralNetwork::InitWeights(float value)
{
	for (auto& layer : m_Layers)
	{

		layer.matrix.leftCols(layer.matrix.cols() - 1).setConstant(value);
	}

}

void NeuralNetwork::InitWeights(std::function<float()> NullaryFunction)
{
	for (auto& layer : m_Layers)
	{
		auto block = layer.matrix.leftCols(layer.matrix.cols() - 1);
		block = block.NullaryExpr(block.rows(), block.cols(), NullaryFunction);
	}
}

void NeuralNetwork::InitBiases(float value)
{
	for (auto& layer : m_Layers)
	{
		layer.matrix.rightCols(1).setConstant(value);
	}
}

void NeuralNetwork::InitBiases(std::function<float()> NullaryFunction)
{
	for (auto& layer : m_Layers)
	{
		auto block = layer.matrix.rightCols(1);
		block = block.NullaryExpr(block.rows(), block.cols(), NullaryFunction);
	}
}

int NeuralNetwork::GetInputSize() const
{
	return m_Layers[0].matrix.cols() - 1;
}

int NeuralNetwork::GetOutputSize() const
{
	return m_Layers[m_Layers.size() - 1].matrix.rows();
}

VectorXf NeuralNetwork::Calculate(VectorXf input)
{
	for (NNLayer& layer : m_Layers)
	{
		if (input.size() != layer.matrix.cols() - 1)
		{
			throw "the size of the input vector does not match the layer matrix!\n";
		}

		//todo: optimization suggestion: use the stored input vector in the layer and copy the passed input vecor over to it
		//layer.inputVector.topRows(layer.inputVector.rows() - 1) = input;

		//resize the input vector to add a 1 
		input.conservativeResize(input.size() + 1);
		input(input.size() - 1) = m_BiasInputConstant;

		//compute weight matrix
		input = layer.matrix * input;

		//activation function
		input = input.unaryExpr(layer.activation);


	}

	return input;
}

