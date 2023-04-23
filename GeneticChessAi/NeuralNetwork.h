#pragma once
#include <initializer_list>
#include <Eigen/Dense>
#include <functional>
#include "ActivationFunctions.h"


using Eigen::MatrixXf;
using Eigen::VectorXf;


enum class ActivationFunc
{
	None,
	Sigmoid,
	Tanh,
	ReLU
};

struct LayerSettings
{
	int nrNodes;
	ActivationFunc activationFunc;
};

class NeuralNetwork final
{
public:
	//first element in array is input layer, last is output layer, the rest are the hidden layers
	NeuralNetwork(const std::vector<LayerSettings>& layerSettings, float value);
	NeuralNetwork(const std::vector<LayerSettings>& layerSettings, const std::vector<std::vector<float>>& data);
	NeuralNetwork(const NeuralNetwork& other);
	NeuralNetwork(const NeuralNetwork&& other)noexcept;
	NeuralNetwork& operator=(const NeuralNetwork& other);
	NeuralNetwork& operator=(NeuralNetwork&& other)noexcept;

	void PrintMatrices() const;

	void InitWeights(float value);
	void InitWeights(std::function<float()> NullaryFunction);

	void InitBiases(float value);
	void InitBiases(std::function<float()> NullaryFunction);

	int GetInputSize() const;
	int GetOutputSize() const;
	std::vector<int> GetLayerSizes() const;
	std::vector<ActivationFunc> GetLayerActivationFunctions() const;
	std::vector<MatrixXf> GetLayerMatrices() const;

	void Save(std::ostream& os);
	static NeuralNetwork Load(std::istream& is);

	bool operator==(const NeuralNetwork& other);


	//a value of 1.f will be appended to the input automatically for the bias layer.
	VectorXf Calculate(VectorXf input);


private:

	struct NNLayer
	{
		MatrixXf matrix;
		VectorXf inputVector;
		ActivationFunc activationEnum;
		std::function<float(float)> activation;
	};


	//the last column of every layer matrix are 
	std::vector<NNLayer> m_Layers;
	const float m_BiasInputConstant{ 1.f };
};

