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

#include "NeuralNetwork.h"

#include <Eigen/Dense>
using Eigen::MatrixXf;
using Eigen::VectorXf;

int main(int argc, char** argv)
{
	NeuralNetwork nn{ {{768, ActivationFunc::None} ,{1, ActivationFunc::ReLU} }, 0.f };
	NNEval eval{ &nn };

	thc::ChessRules pos{};
	auto input = eval.EncodeInput(pos);

	for (int i{}; i < input.size(); i++)
	{
		if (i % 8 == 0) std::cout << std::endl;
		if (i % 64 == 0) std::cout << std::endl;

		std::cout << std::right << std::setprecision(2) << std::setw(2) << input[i] << ' ';
	}

	/*

	//random init test
	std::random_device rd;
	std::uniform_real_distribution<float> rw(-0.25f, 0.25f);
	std::uniform_real_distribution<float> rb{ -1.f, 1.f };
	std::uniform_int_distribution<int>inputDistr{ -1, 1 };

	auto randomWeight = [&rd, &rw]() {return rw(rd); };
	auto randomBias = [&rd, &rb]() {return rb(rd); };
	auto randomInput = [&rd, &inputDistr]() {return inputDistr(rd); };

	//NeuralNetwork nn{ {{768, ActivationFunc::ReLU} ,{128, ActivationFunc::ReLU},{16, ActivationFunc::ReLU},{1, ActivationFunc::Tanh}}, 1.f };
	NeuralNetwork nn{ {{3, ActivationFunc::ReLU} ,{5, ActivationFunc::ReLU},{1, ActivationFunc::Tanh}}, 1.f };
	NeuralNetwork nn2{ {{2, ActivationFunc::ReLU} ,{3, ActivationFunc::ReLU},{1, ActivationFunc::Tanh}}, 1.f };

	nn.InitBiases(std::function<float()>(randomBias));
	nn.InitWeights(randomWeight);

	std::ofstream output{ "testfile.txt" };
	if (output)
	{
		std::cout << "MATRIX BEFORE SAVING" << std::endl;
		nn.PrintMatrices();


		nn.Save(output);

		output.close();
	}


	std::ifstream input{ "testfile.txt" };
	if (input)
	{
		std::cout << "MATRIX AFTER LOADING" << std::endl;
		NeuralNetwork nnLoad = NeuralNetwork::Load(input);
		nnLoad.PrintMatrices();


		std::cout << (nn == nnLoad ? "MATRICES EQUAL" : "MATRICES NOT EQUAL") << std::endl;
	}
	*/


	/*
	VectorXf input{ nn.GetInputSize() };
	input = input.NullaryExpr(input.rows(), input.cols(), randomInput);

	VectorXf output = nn.Calculate(input);
	std::cout << output << std::endl;
	*/


	/*
	std::cout << m;

	thc::ChessRules pos{};
	pos.Forsyth("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w - - 0 1");

	MaterialEval eval{};
	MiniMaxPlayer miniMax{ 4, eval };

	std::vector<long long> timings;
	std::vector<int> treeSizes;
	int nrSamples{ 10 };
	for (int i{}; i < nrSamples; ++i)
	{
		auto start = std::chrono::high_resolution_clock::now();
		miniMax.MakeMove(pos);
		auto end = std::chrono::high_resolution_clock::now();

		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

		timings.push_back(duration.count());
		treeSizes.push_back(miniMax.LeafNodeCount());

	}

	for (int i{}; i < nrSamples; ++i)
	{
		std::cout << "thinking time: " << std::setw(6) << timings[i] << "ms" << std::endl;
	}

	std::cout << std::endl;
	std::cout << "avg thinking time: " << std::setw(6)  << std::accumulate(timings.begin(), timings.end(),0) / timings.size() << "ms" << std::endl;
	std::cout << "positions evaluated: " << std::setw(30) << std::left << treeSizes[0] << std::endl;

	*/



	/*
		MaterialEval evalFunc{};
		MiniMaxPlayer botWhite{4, evalFunc};

		RandomBot botBlack{};

		ChessGame game{&botWhite, &botBlack, false };
		//game.setStartingPos("8/7k/1R6/RK6/8/8/8/8 w - - 0 1");

		game.PlayGame();

		switch (game.GetTerminalState())
		{
		case thc::TERMINAL_WCHECKMATE:
			std::cout << "BLACK WON\n";
			break;
		case thc::TERMINAL_WSTALEMATE:
			std::cout << "STALEMATE\n";
			break;
		case thc::TERMINAL_BCHECKMATE:
			std::cout << "WHITE WON\n";
			break;
		case thc::TERMINAL_BSTALEMATE:
			std::cout << "STALEMATE\n";
			break;
		default:
			break;
		}

		switch (game.GetDrawState())
		{
		case thc::DRAWTYPE_50MOVE:
			std::cout << "50 MOVE RULE DRAW\n";
			break;
		case thc::DRAWTYPE_INSUFFICIENT:
		case thc::DRAWTYPE_INSUFFICIENT_AUTO:
			std::cout << "INSUFICIENT MATERIAL DRAW\n";
			break;
		case thc::DRAWTYPE_REPITITION:
			std::cout << "REPITITION DRAW\n";
			break;
		default:
			break;
		}
			std::cout << game.GetMovesRecord();
	*/
}

