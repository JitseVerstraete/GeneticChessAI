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

#include "TranspositionTable.h"

#include <Eigen/Dense>
using Eigen::MatrixXf;
using Eigen::VectorXf;


int main(int, char**)
{
	/*
		//generate and save new neural network
		NeuralNetwork nn{ {{ 768, ActivationFunc::None}, {64, ActivationFunc::ReLU}, {16, ActivationFunc::ReLU}, {1, ActivationFunc::Tanh}} , 0.f };
		nn.InitBiasesRandom(-0.5f, 0.5f);
		nn.InitWeightsRandom(-0.5f, 0.5f);

		std::ofstream file{ "testnn.txt" };
		nn.Save(file);
	*/
	
	//do timings on the playing of 1 move
	thc::ChessRules pos{};
	pos.Forsyth("2kr3r/ppp1ppbp/2n1qnp1/1N3b2/8/1P1B1N2/PBPPQPPP/R4RK1 b - - 5 10");

	std::ifstream file{ "testnn.txt" };
	NeuralNetwork nn = NeuralNetwork::Load(file);


	NNEval eval{ &nn };
	MiniMaxPlayer<NNEval> player{ 4, eval, 1'000'000 };

	float totalTime{};
	for (int i{}; i < 100; i++)
	{
		Timer timer{};
		std::cout << player.MakeMove(pos).NaturalOut(&pos) << std::endl;
		totalTime += timer.GetDuration<std::milli>();
		std::cout << "nodes evaluated:" << player.LeafNodeCount() << std::endl;
	}
	std::cout << std::endl << "average move time: " << totalTime / 100 << "ms\n";

	
	



	/*
	{

		thc::ChessRules pos{};
		thc::Move move{};

		uint64_t hash{};

		hash = pos.Hash64Calculate();
		std::cout << "initial position hash:\n";
		std::cout << hash << std::endl << std::endl;;

		move.NaturalIn(&pos, "e4");
		hash = pos.Hash64Update(hash, move);
		pos.PlayMove(move);


		move.NaturalIn(&pos, "e5");
		hash = pos.Hash64Update(hash, move);
		pos.PlayMove(move);

		move.NaturalIn(&pos, "d4");
		hash = pos.Hash64Update(hash, move);
		pos.PlayMove(move);


		move.NaturalIn(&pos, "d5");
		hash = pos.Hash64Update(hash, move);
		pos.PlayMove(move);

		std::cout << "hash after one move recomputed:\n";
		std::cout << pos.Hash64Calculate() << std::endl;
		std::cout << "hash after one move updated:\n";
		std::cout << hash << std::endl << std::endl;;

	}


	{

		thc::ChessRules pos{};
		thc::Move move{};

		uint64_t hash{};

		hash = pos.Hash64Calculate();
		std::cout << "initial position hash:\n";
		std::cout << hash << std::endl << std::endl;;

		move.NaturalIn(&pos, "d3");
		hash = pos.Hash64Update(hash, move);
		pos.PlayMove(move);

		move.NaturalIn(&pos, "d6");
		hash = pos.Hash64Update(hash, move);
		pos.PlayMove(move);

		move.NaturalIn(&pos, "d4");
		hash = pos.Hash64Update(hash, move);
		pos.PlayMove(move);

		move.NaturalIn(&pos, "d5");
		hash = pos.Hash64Update(hash, move);
		pos.PlayMove(move);


		move.NaturalIn(&pos, "e3");
		hash = pos.Hash64Update(hash, move);
		pos.PlayMove(move);


		move.NaturalIn(&pos, "e6");
		hash = pos.Hash64Update(hash, move);
		pos.PlayMove(move);

		move.NaturalIn(&pos, "e4");
		hash = pos.Hash64Update(hash, move);
		pos.PlayMove(move);


		move.NaturalIn(&pos, "e5");
		hash = pos.Hash64Update(hash, move);
		pos.PlayMove(move);



		std::cout << "hash after one move recomputed:\n";
		std::cout << pos.Hash64Calculate() << std::endl;
		std::cout << "hash after one move updated:\n";
		std::cout << hash << std::endl;

	}
	*/


	/*
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

	*/


	/*
	GeneticSettings settings{};
	settings.maxGenerations = 100;
	settings.gamesPlayed = 2;
	settings.threads = 12;
	settings.elitismSize = 3;
	settings.saveFrequency = 9;
	settings.PopulationName = "testGen";
	GeneticAlgorithm ga{ settings };

	//NeuralNetwork nnTemplate{ {{ 768, ActivationFunc::None}, {128, ActivationFunc::ReLU}, {32, ActivationFunc::ReLU}, {1, ActivationFunc::Tanh}} , 0.f };
	NeuralNetwork nnTemplate{ {{768, ActivationFunc::None}, {2, ActivationFunc::ReLU}, {2, ActivationFunc::ReLU}, {1, ActivationFunc::Tanh}} , 0.f };
	ga.InitializeNewPopulation(nnTemplate, 10, 0.5f, 0.5f);
	ga.Run();



	std::ifstream inputSettings{ "GA-Output/testGen/GA-Settings.txt" };
	GeneticAlgorithm loadedGA{ inputSettings };
	std::ifstream inputPopulation{ "GA-Output/testGen/Generation-100.txt" };
	loadedGA.InitializePopulationFromFile(inputPopulation);

	std::cout << "done";
	*/

}
