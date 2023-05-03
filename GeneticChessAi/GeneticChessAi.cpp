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


