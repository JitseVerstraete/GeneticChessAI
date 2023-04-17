#pragma once
#include <initializer_list>
#include <Eigen/Dense>

using Eigen::MatrixXf;
using Eigen::VectorXf;

class NeuralNetwork
{
public:
	//first element in array is input layer, last is output layer, the rest are the hidden layers
	NeuralNetwork( std::initializer_list<int> layerSizes, float value);


	void PrintMatrices() const;

	int GetInputSize() const;
	int GetOutputSize() const;
	VectorXf Calculate(const VectorXf& input) const;
	

private:
	std::vector<MatrixXf> m_Layers;

};

