#include "GeneticAlgorithm.h"
#include <numeric>
#include <iostream>
#include <random>
#include "MiniMaxPlayer.h" 
#include "ChessGame.h"



GeneticAlgorithm::GeneticAlgorithm(const GeneticSettings& settings)
	:m_Settings{ settings }
{
	srand(unsigned int(time(NULL)));
	//if the mutation max setting is negative, turn it positive
	m_Settings.mutationMax = std::abs(m_Settings.mutationMax);
}

void GeneticAlgorithm::InitializeExistingPopulation(const std::vector<NeuralNetwork>& initialPopulation)
{
	for (const NeuralNetwork& nn : initialPopulation)
	{
		m_Individuals.push_back(std::make_shared<GeneticAlgorithm::Individual>(GeneticAlgorithm::Individual(std::make_unique<NeuralNetwork>(nn), 0.f, 0.f)));
	}
}

void GeneticAlgorithm::InitializeNewPopulation(const NeuralNetwork& networkTemplate, int populationSize, float weightInitRange, float biasInitRange)
{
	for (int i{}; i < populationSize; ++i)
	{
		NeuralNetwork nn = networkTemplate;
		nn.InitWeightsRandom(-std::abs(weightInitRange), std::abs(weightInitRange));
		nn.InitBiasesRandom(-std::abs(biasInitRange), std::abs(biasInitRange));
		m_Individuals.push_back(std::make_shared<GeneticAlgorithm::Individual>(GeneticAlgorithm::Individual(std::make_unique<NeuralNetwork>(nn), 0.f, 0.f)));
	}

}

void GeneticAlgorithm::Run()
{
	for (int generationCounter{}; generationCounter < m_Settings.maxGenerations; ++generationCounter)
	{

		EvaluateFitness();

		//select partents
		std::vector<std::pair<IndividualPtr, IndividualPtr>> parents = SelectParents();

		for (int i{}; i < parents.size(); ++i)
		{
			//create new child through crossover
			auto child = Crossover(parents[i].first->pNetwork.get(), parents[i].second->pNetwork.get());
			//mutate the values of the child
			Mutate(child);

			m_Individuals[i]->pNetwork = std::make_unique<NeuralNetwork>(child);
		}

		//reset fitness values
		ResetFitness();

		std::cout << "Generation " << generationCounter << '/' << m_Settings.maxGenerations << std::endl;
	}
}

void GeneticAlgorithm::ResetFitness()
{
	for (auto& individual : m_Individuals)
	{
		individual->fitness = 0.f;
		individual->weight = 0.f;
	}
}

void GeneticAlgorithm::EvaluateFitness()
{
	ResetFitness();

	//create matches, first is white, black is second
	std::vector<std::pair<std::shared_ptr<Individual>, std::shared_ptr<Individual>>> gamePairings;

	for (int playerIndex{}; playerIndex < m_Individuals.size(); playerIndex++)
	{
		for (int gameNr{}; gameNr < m_Settings.gamesPlayed;)
		{
			int opponentIndex = rand() % m_Individuals.size();

			if (playerIndex != opponentIndex)
			{
				std::shared_ptr<Individual> player{ m_Individuals[playerIndex] };
				std::shared_ptr<Individual>  opponent{ m_Individuals[opponentIndex] };
				auto gamePair = (gameNr < m_Settings.gamesPlayed / 2) ?
					std::pair(std::shared_ptr<Individual>(player), std::shared_ptr<Individual>(opponent)) :
					std::pair(std::shared_ptr<Individual>(opponent), std::shared_ptr<Individual>(player));

				gamePairings.push_back(gamePair);

				//only increment the game nr if a match was successfully created
				gameNr++;
			}
		}
	}




	//play matches
	//todo: multithread playing the matches


	//the first of the pair is white, the second in black
	for (auto& pairing : gamePairings)
	{
		MiniMaxPlayer<NNEval> wPlayer{ m_Settings.minMaxDepth, NNEval(pairing.first->pNetwork.get()) };
		MiniMaxPlayer<NNEval> bPlayer{ m_Settings.minMaxDepth, NNEval(pairing.second->pNetwork.get()) };

		ChessGame game = ChessGame(&wPlayer, &bPlayer, false);
		game.PlayGame();
		GameResult result = game.GetGameResult();

		switch (result)
		{
		case GameResult::Draw:
			pairing.first->fitness += 0.5f;
			pairing.second->fitness += 0.5f;
			break;
		case GameResult::WhiteWin:
			pairing.first->fitness += 1.f;
			break;
		case GameResult::BlackWin:
			pairing.second->fitness += 1.f;
			break;
		case GameResult::NoResult:
			assert(false && "it should be impossible for the game to have no result after playing!");
			break;
		default:
			break;
		}

	}

}

std::vector<std::pair<GeneticAlgorithm::IndividualPtr, GeneticAlgorithm::IndividualPtr>> GeneticAlgorithm::SelectParents()
{
	std::vector<std::pair<IndividualPtr, IndividualPtr>> parents{};

	float totalFitness{};
	for (const auto& ind : m_Individuals)
	{
		totalFitness += ind->fitness;
	}

	for (auto& ind : m_Individuals)
	{
		ind->weight = ind->fitness / totalFitness;
	}



	IndividualPtr pParent1{};
	IndividualPtr pParent2{};

	while (parents.size() < m_Individuals.size())
	{
		do
		{
			pParent1 = PickIndividual();
			pParent2 = PickIndividual();

		} while (pParent1->pNetwork == pParent2->pNetwork);

		//add the parents to the 
		parents.push_back(std::pair<IndividualPtr, IndividualPtr>(pParent1, pParent2));
	}

	return parents;
}

GeneticAlgorithm::IndividualPtr GeneticAlgorithm::PickIndividual()
{
	float currentValue{};

	std::random_device rd;
	std::uniform_real_distribution distr{ 0.f, 1.f };

	float targetValue{ distr(rd) };

	for (const auto& ind : m_Individuals)
	{
		currentValue += ind->weight;
		if (targetValue <= currentValue)
		{
			return ind;
		}
	}

	throw std::exception("something went wrong picking an individual");
}



NeuralNetwork GeneticAlgorithm::Crossover( NeuralNetwork* parent1, NeuralNetwork* parent2)
{
	//uniform crossover
	NeuralNetwork childNetwork = *parent1;


	for (int index{}; index < childNetwork.GetNrLayerMatrices(); index++)
	{
		MatrixXf& matrix = childNetwork.GetLayerMatrix(index);

		for (int row{}; row < matrix.rows(); ++row)
		{
			for (int col{}; col < matrix.cols(); ++col)
			{
				matrix(row, col) = (rand() % 2 == 0) ? parent1->GetLayerMatrix(index)(row, col) : parent2->GetLayerMatrix(index)(row, col);
			}
		}
	}

	return childNetwork;
}


void GeneticAlgorithm::Mutate(NeuralNetwork& network)
{
	std::random_device rd;
	std::uniform_real_distribution uniDistr{ 0.f, 1.f };
	std::normal_distribution normalDistr{ 0.f, m_Settings.mutationDeviation };


	for (int index{}; index < network.GetNrLayerMatrices(); index++)
	{
		for (auto& value : network.GetLayerMatrix(index).reshaped())
		{
			float rand{ uniDistr(rd) };
			if (rand < m_Settings.mutationChance)
			{
				//mutate the gene (adding random generated value to the weight/bias)
				value += std::clamp(normalDistr(rd), -m_Settings.mutationMax, m_Settings.mutationMax);
			}

		}
	}
}
