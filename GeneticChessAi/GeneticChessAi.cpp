// GeneticChessAi.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define _CRT_SECURE_NO_WARNINGS

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
#include <algorithm>
#include <random>
#include <fstream>
#include "GeneticAlgorithm.h"

#include "NeuralNetwork.h"
#include <memory>
#include "Timer.h"
#include <ctime>
#include <filesystem>
#include <bitset>
#include <chrono>
#include "TranspositionTable.h"

#include <Eigen/Dense>
using Eigen::MatrixXf;
using Eigen::VectorXf;


int main(int, char**)
{

	/*
		GeneticAlgorithm testRun1000{ "NoCrossRankHighMutation" };
		testRun1000.InitializePopulationFromFile("NoCrossRankHighMutation", 1000);

		GeneticAlgorithm testRun0{ "NoCrossRankHighMutation" };
		testRun0.InitializePopulationFromFile("NoCrossRankHighMutation", 0);

		MatchResults result =  testRun1000.Compare(testRun0, 3);

		std::cout << "wins: " << result.wins << std::endl;
		std::cout << "draws: " << result.draws << std::endl;
		std::cout << "losses: " << result.losses << std::endl;
	*/


	
	GeneticSettings settings{};
	settings.PopulationName = "NoCrossoverRankSelection2";

	settings.maxGenerations = 1000;
	settings.threads = 12;
	settings.saveFrequency = 50;

	settings.selection = SelectionType::Rank;
	settings.gamesPlayed = 10; //every player will end up playing double this, because opponents will challenge them
	settings.minMaxDepth = 4;
	settings.ttSize = 1'000'000; //24MB per player * 24max players at any time = 576MB maximum
	settings.elitismSize = 3;

	settings.crossover = CrossoverType::None;

	settings.mutationChance = 0.05f;
	settings.mutationDeviation = 0.5f;

	GeneticAlgorithm ga{ settings };

	NeuralNetwork nnTemplate{ {{768, ActivationFunc::None}, {64, ActivationFunc::ReLU}, {16, ActivationFunc::ReLU}, {1, ActivationFunc::Tanh}} , 0.f };
	ga.InitializeNewPopulation(nnTemplate, 25, 0.5f, 0.5f);


	Timer timer{};
	ga.Run();
	std::cout << std::endl << "total training time: " << timer.GetDuration<std::ratio<3600, 1>>() << " hours\n";
	

}
