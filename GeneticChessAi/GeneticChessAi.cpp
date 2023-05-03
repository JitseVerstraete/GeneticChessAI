// GeneticChessAi.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <thc.h>
#include "ChessGUI.h"
#include "ChessGame.h"
#include "HumanPlayer.h"
#include "RandomBot.h"
#include "EvalFunctions.h"
#include "MiniMaxPlayer.h"
#include <iomanip>
#include <numeric>
#include <random>
#include <fstream>
#include "GeneticAlgorithm.h"

#include "NeuralNetwork.h"
#include <memory>
#include "Timer.h"

#include <Eigen/Dense>
using Eigen::MatrixXf;
using Eigen::VectorXf;


int main(int, char**)
{
	//todo: NeuralNetwork::Calculate, pass the input as const ref (use the layer stored input vector to map the values of the input parameter or output of the previous result)

	/*
	NeuralNetwork nn{ {{ 2, ActivationFunc::None}, {2, ActivationFunc::ReLU}, {1, ActivationFunc::Tanh}} , 0.f };

	nn.InitBiasesRandom(-0.5f, 0.5f);
	nn.InitWeightsRandom(-0.5f, 0.5f);

	VectorXf input(2);
	input.setConstant(3.f);
	float value = nn.Calculate(input)[0];
	std::cout << value << std::endl;
	value = nn.CalculateV2(input)[0];
	std::cout << value << std::endl;
	value = nn.CalculateV3(input)[0];
	std::cout << value << std::endl;
	*/







	NeuralNetwork nn{ {{ 768, ActivationFunc::None}, {64, ActivationFunc::ReLU}, {16, ActivationFunc::ReLU}, {1, ActivationFunc::Tanh}} , 0.f };
	nn.InitBiasesRandom(-0.5f, 0.5f);
	nn.InitWeightsRandom(-0.5f, 0.5f);

	NNEval eval{ &nn };
	MiniMaxPlayer white{ 4, eval };

	thc::ChessRules pos{};

	int iterations{ 100 };

	float value{};
	float totalTime{};
	Timer timer{};


	totalTime = 0;
	for (int i{}; i < iterations; ++i)
	{

		timer.Start();
		white.MakeMove(pos);
		float time = timer.GetDuration<std::milli>();
		totalTime += time;
	}
	std::cout << "average = " << totalTime / iterations << std::endl;




	/*
	GeneticSettings settings{};
	settings.maxGenerations = 10;
	settings.gamesPlayed = 2;
	settings.threads = 12;
	GeneticAlgorithm ga{ settings };

	NeuralNetwork nnTemplate{ {{ 768, ActivationFunc::None}, {128, ActivationFunc::ReLU}, {32, ActivationFunc::ReLU}, {1, ActivationFunc::Tanh}} , 0.f };
	ga.InitializeNewPopulation(nnTemplate, 10, 0.5f, 0.5f);

	ga.Run();
	*/
}


