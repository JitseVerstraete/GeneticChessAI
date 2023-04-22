#pragma once
#include <initializer_list>
#include <Eigen/Dense>
#include <functional>
#include "ActivationFunctions.h"

using Eigen::MatrixXf;
using Eigen::VectorXf;

struct LayerSettings
{
	int nrNodes;
	std::function<float(float)> activationFunc;
};

class NeuralNetwork
{
public:
	//first element in array is input layer, last is output layer, the rest are the hidden layers
	NeuralNetwork(const std::vector<LayerSettings>&, float value);

	void PrintMatrices() const;

	void InitWeights(float value);
	void InitWeights(std::function<float()> NullaryFunction);

	void InitBiases(float value);
	void InitBiases(std::function<float()> NullaryFunction);



	int GetInputSize() const;
	int GetOutputSize() const;

	//a value of 1.f will be appended to the input automatically for the bias layer.
	VectorXf Calculate(VectorXf input);


private:
	
	struct NNLayer
	{
		MatrixXf matrix;
		VectorXf inputVector;
		std::function<float(float)> activation;
	};


	//the last column of every layer matrix are 
	std::vector<NNLayer> m_Layers;
	const float m_BiasInputConstant{ 1.f };
};

