#pragma once
#include <initializer_list>
#include <Eigen/Dense>
#include <functional>
#include "ActivationFunctions.h"

using Eigen::MatrixXf;
using Eigen::VectorXf;

class NeuralNetwork
{
public:
	//first element in array is input layer, last is output layer, the rest are the hidden layers
	NeuralNetwork( std::initializer_list<int> layerSizes, std::function<float(float)> activationFunc, float value);


	void PrintMatrices() const;

	MatrixXf GetLayerWeights(int layer);
	VectorXf GetLayerBiases(int layer);

	void InitWeights(float value);
	void InitWeights(std::function<float()> NullaryFunction);
	
	void InitBiases(float value);
	void InitBiases(std::function<float()> NullaryFunction);

	

	int GetInputSize() const;
	int GetOutputSize() const;

	//a value of 1.f will be appended to the input automatically for the bias layer.
	VectorXf Calculate(VectorXf input) const;
	

private:
	//the last column of every layer matrix are 
	std::vector<MatrixXf> m_Layers;
	const float m_BiasInputConstant{ 1.f };

	std::function<float(float)> m_ActivationFunction;
};

