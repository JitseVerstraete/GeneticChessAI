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
	GeneticAlgorithm testRun1000{ "FirstFullTestRun" };
	testRun1000.InitializePopulationFromFile("FirstFullTestRun", 1000);

	GeneticAlgorithm testRun0{ "FirstFullTestRun" };
	testRun0.InitializePopulationFromFile("FirstFullTestRun", 0);

	MatchResults result =  testRun1000.Compare(testRun0, 1);

	std::cout << "wins: " << result.wins << std::endl;
	std::cout << "draws: " << result.draws << std::endl;
	std::cout << "losses: " << result.losses << std::endl;


	/*
	GeneticSettings settings{};
	settings.PopulationName = "FirstFullTestRun";

	settings.maxGenerations = 1000;
	settings.threads = 12;
	settings.saveFrequency = 50;

	settings.gamesPlayed = 4; //every player will end up playing double this, because opponents will challenge them
	settings.minMaxDepth = 4;
	settings.elitismSize = 3;
	settings.ttSize = 1'000'000; //24MB per player * 24max players at any time = 576MB maximum

	settings.mutationChance = 0.05f;
	settings.mutationDeviation = 0.5f;

	GeneticAlgorithm ga{ settings };

	NeuralNetwork nnTemplate{ {{768, ActivationFunc::None}, {64, ActivationFunc::ReLU}, {16, ActivationFunc::ReLU}, {1, ActivationFunc::Tanh}} , 0.f };
	ga.InitializeNewPopulation(nnTemplate, 50, 0.5f, 0.5f);


	Timer timer{};
	ga.Run();
	std::cout << std::endl << "total training time: " << timer.GetDuration<std::ratio<3600, 1>>() << " hours\n";
	*/



	/*
	GeneticSettings settings{};
	settings.PopulationName = "PopLoad";

	settings.maxGenerations = 100;
	settings.threads = 12;
	settings.saveFrequency = 10;

	settings.gamesPlayed = 4; //every player will end up playing double this, because opponents will challenge them
	settings.minMaxDepth = 2;
	settings.ttSize = 1'000'000; //24MB per player * 24max players at any time = 576MB maximum
	settings.elitismSize = 3;

	settings.mutationChance = 0.05f;
	settings.mutationDeviation = 0.5f;

	GeneticAlgorithm ga{ settings };
	GeneticAlgorithm ga2{ settings };

	NeuralNetwork nnTemplate{ {{768, ActivationFunc::None}, {64, ActivationFunc::ReLU}, {16, ActivationFunc::ReLU}, {1, ActivationFunc::Tanh}} , 0.f };
	ga.InitializeNewPopulation(nnTemplate, 10, 0.5f, 0.5f);

	ga.PrepOutputFolder();
	ga.SaveGeneration();

	ga2.InitializePopulationFromFile("PopLoad", 0);


	MatchResults result = ga.Compare(ga2, 4);

	std::cout << "wins: " << result.wins << std::endl;
	std::cout << "draws: " << result.draws << std::endl;
	std::cout << "losses: " << result.losses << std::endl;
	*/



	//WRITE SOME NN TO FILE (TEST)
	/*
	for (int i{}; i < 10; i++)
	{
		//generate and save new neural network
		NeuralNetwork nn{ {{ 768, ActivationFunc::None}, {64, ActivationFunc::ReLU}, {16, ActivationFunc::ReLU}, {1, ActivationFunc::Tanh}} , 0.f };
		nn.InitBiasesRandom(-0.5f, 0.5f);
		nn.InitWeightsRandom(-0.5f, 0.5f);
		std::string fileName = "testnn";
		fileName.append(std::to_string(i));
		fileName.append(".txt");
		std::ofstream file{ fileName};
		nn.Save(file);
	}
	*/

	//READ SOME NN FROM FILE (TEST)
	/*
	for (int i{}; i < 10; i++)
	{

		//do timings on the playing of 1 move
		thc::ChessRules pos{};
		pos.Forsyth("2kr3r/ppp1ppbp/2n1qnp1/1N3b2/8/1P1B1N2/PBPPQPPP/R4RK1 b - - 5 10");

		std::string fileName = "testnn";
		fileName.append(std::to_string(i));
		fileName.append(".txt");

		std::ifstream file{ fileName };
		NeuralNetwork nn = NeuralNetwork::Load(file);


		NNEval eval{ &nn };
		MiniMaxPlayer<NNEval> player{ 4, eval, 1'000'000 };
		thc::Move move{};

		float totalTime{};
		for (int i{}; i < 50; i++)
		{
			Timer timer{};
			move = player.MakeMove(pos);
			totalTime += timer.GetDuration<std::milli>();

		}
		std::cout << "Move Played: " << move.NaturalOut(&pos) << std::endl;
		std::cout << "nodes evaluated:" << player.LeafNodeCount() << std::endl;
		std::cout << "transpositions found:" << player.TranspositionCount() << std::endl;
		std::cout << "average move time: " << totalTime / 50 << "ms\n" << std::endl;
	}
	*/





	//HASH TEST
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
}
