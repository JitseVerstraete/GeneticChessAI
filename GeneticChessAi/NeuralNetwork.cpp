#include "NeuralNetwork.h"

#include <random>
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

		/*
		VectorXf inputVec(matrix.cols());
		inputVec.setConstant(0);
		inputVec(inputVec.rows() - 1) = m_BiasInputConstant;
		*/

		std::function<float(float)> actFunc{};
		ActivationFunc funcEnum = (it + 1)->activationFunc;
		switch (funcEnum)
		{
		case ActivationFunc::Sigmoid:
			actFunc = Sigmoid;
			break;
		case ActivationFunc::ReLU:
			actFunc = ReLU;
			break;
		case ActivationFunc::Tanh:
			actFunc = Tanh;
			break;
		default:
			throw "This activation function is not implemented yet";
			break;
		}

		m_Layers.emplace_back(NNLayer{ std::move(matrix), funcEnum, actFunc });
	}

}

NeuralNetwork::NeuralNetwork(const std::vector<LayerSettings>& layerSettings, const std::vector<std::vector<float>>& data)
	:NeuralNetwork(layerSettings, 0.f)
{
	if (m_Layers.size() != data.size())
	{
		throw "the amount of data arrays given doesn't match the amount of layer matrices!";
	}

	for (int i{}; i < m_Layers.size(); ++i)
	{
		if (m_Layers[i].matrix.size() != data[i].size())
		{
			throw "the amount of data given to a matrix doesnt match the matrix size!";
		}

		m_Layers[i].matrix = MatrixXf::Map(data[i].data(), m_Layers[i].matrix.rows(), m_Layers[i].matrix.cols());
	}



}

NeuralNetwork::NeuralNetwork(const NeuralNetwork& other)
	:m_Layers{ other.m_Layers }
{
}

NeuralNetwork::NeuralNetwork(const NeuralNetwork&& other) noexcept
	:m_Layers{ other.m_Layers }
{
}

NeuralNetwork& NeuralNetwork::operator=(const NeuralNetwork& other)
{
	return *this = NeuralNetwork(other);
}

NeuralNetwork& NeuralNetwork::operator=(NeuralNetwork&& other)noexcept
{
	m_Layers = other.m_Layers;
	return *this;
}

void NeuralNetwork::PrintMatrices() const
{
	for (int i{}; i < m_Layers.size(); ++i)
	{
		std::cout << "Layer " << i << ": " << "in(" << m_Layers[i].matrix.cols() - 1 << ")" << "out(" << m_Layers[i].matrix.rows() << ")" << std::endl;
		std::cout << m_Layers[i].matrix << std::endl;
		std::cout << "activation func id:" << (int)m_Layers[i].activationEnum << std::endl << std::endl;
	}
}

void NeuralNetwork::SetConstant(float value)
{
	for (auto& layer : m_Layers)
	{
		layer.matrix.setConstant(value);
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

void NeuralNetwork::InitWeightsRandom(float min, float max)
{
	std::random_device rd;
	std::uniform_real_distribution<float> weightDistr(min, max);

	auto randomWeights = [&rd, &weightDistr]() {return weightDistr(rd); };
	InitWeights(randomWeights);
}



void NeuralNetwork::InitBiases(std::function<float()> NullaryFunction)
{
	for (auto& layer : m_Layers)
	{
		auto block = layer.matrix.rightCols(1);
		block = block.NullaryExpr(block.rows(), block.cols(), NullaryFunction);
	}
}

void NeuralNetwork::InitBiasesRandom(float min, float max)
{
	std::random_device rd;
	std::uniform_real_distribution<float> biasDistr(min, max);

	auto randomBiases = [&rd, &biasDistr]() {return biasDistr(rd); };
	InitBiases(randomBiases);

}

int NeuralNetwork::GetInputSize() const
{
	return m_Layers[0].matrix.cols() - 1;
}

int NeuralNetwork::GetOutputSize() const
{
	return m_Layers[m_Layers.size() - 1].matrix.rows();
}

std::vector<int> NeuralNetwork::GetLayerSizes() const
{
	std::vector<int> toReturn{};

	//add the input size of every matrix
	for (int i{}; i < m_Layers.size(); ++i)
	{
		toReturn.push_back(m_Layers[i].matrix.cols() - 1);
	}

	//add output size
	toReturn.push_back(m_Layers[m_Layers.size() - 1].matrix.rows());

	return toReturn;
}

std::vector<ActivationFunc> NeuralNetwork::GetLayerActivationFunctions() const
{
	std::vector<ActivationFunc> toReturn{};

	for (int i{}; i < m_Layers.size(); ++i)
	{
		toReturn.push_back(m_Layers[i].activationEnum);
	}

	return toReturn;
}

std::vector<MatrixXf> NeuralNetwork::GetLayerMatrices() const
{
	std::vector<MatrixXf> toReturn{};

	for (const auto& layer : m_Layers)
	{
		toReturn.push_back(layer.matrix);
	}

	return toReturn;
}


MatrixXf& NeuralNetwork::GetLayerMatrix(int index)
{
	if (index < 0 || index >= m_Layers.size())
	{
		throw std::out_of_range("index too big");
	}
	return m_Layers[index].matrix;
}

int NeuralNetwork::GetNrLayerMatrices() const
{
	return m_Layers.size();
}


void NeuralNetwork::Save(std::ostream& os)
{
	//write the sizes of the layers
	for (int size : GetLayerSizes())
	{
		os << size << ' ';
	}
	os << std::endl;

	//write the activation type coded as an int
	for (const ActivationFunc& activation : GetLayerActivationFunctions())
	{
		os << int(activation) << ' ';
	}
	os << std::endl;

	for (const MatrixXf& m : GetLayerMatrices())
	{
		for (float value : m.reshaped())
		{
			os << value << ' ';
		}
		os << std::endl;
	}

	os << std::endl;

}

NeuralNetwork NeuralNetwork::Load(std::istream& is)
{
	std::vector<int> layersizes{};
	std::vector<ActivationFunc> activationFuncs{};
	std::vector<std::vector<float>> matrixcontents{};


	//read layer sizes
	int foundInt{};

	std::string line{};
	std::getline(is, line);
	std::stringstream ss{ line };

	while (ss >> foundInt)
	{
		layersizes.push_back(foundInt);
	}


	//read layer functions
	activationFuncs.push_back(ActivationFunc::None);

	int activationFuncInt{};

	std::getline(is, line);
	ss.clear();
	ss.str(line);

	while (ss >> activationFuncInt)
	{
		activationFuncs.push_back((ActivationFunc)activationFuncInt);
	}


	//read network data
	MatrixXf matrix{};
	std::vector<float> listOfWeights{};

	for (int i{}; i < layersizes.size() - 1; ++i)
	{
		matrix = MatrixXf(layersizes[i + 1], layersizes[i] + 1);
		float weight{};

		std::getline(is, line);
		ss.clear();
		ss.str(line);

		while (ss >> weight)
		{
			listOfWeights.push_back(weight);
		}

		matrixcontents.push_back(listOfWeights);
		listOfWeights.clear();
	}

	//create neural network and move it to the nn variable

	std::vector<LayerSettings> settings{ layersizes.size() };
	for (int i{}; i < settings.size(); ++i)
	{
		settings[i].nrNodes = layersizes[i];
		settings[i].activationFunc = activationFuncs[i];
	}

	return NeuralNetwork(settings, matrixcontents);

}

bool NeuralNetwork::operator==(const NeuralNetwork& other)
{
	//compare sizes
	if (m_Layers.size() != other.m_Layers.size()) return false;

	auto itThis{ m_Layers.begin() };
	auto itOther{ other.m_Layers.begin() };

	//compare matrix contents
	while (itThis != m_Layers.end() && itOther != other.m_Layers.end())
	{
		if (itThis->matrix.rows() != itOther->matrix.rows() || itThis->matrix.cols() != itOther->matrix.cols())
		{
			return false;
		}

		bool matrixequal = itThis->matrix.isApprox(itOther->matrix);
		bool activequal = itThis->activationEnum == itOther->activationEnum;

		if (!matrixequal || !activequal)
		{
			return false;
		}

		itThis++;
		itOther++;
	}

	return true;

}



VectorXf NeuralNetwork::Calculate(VectorXf input)
{
	for (NNLayer& layer : m_Layers)
	{
		if (input.size() != layer.matrix.cols() - 1)
		{
			throw std::exception("the size of the input vector does not match the layer matrix!\n");
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
