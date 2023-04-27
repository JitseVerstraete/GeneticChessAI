#include "GeneticAlgorithm.h"
#include <numeric>
#include <iostream>
#include <random>

GeneticAlgorithm::GeneticAlgorithm(const GeneticSettings& settings)
	:m_Settings{ settings }
{
	srand(unsigned int(time(NULL)));
	//if the mutation max setting is negative, turn it positive
	m_Settings.mutationMax = std::abs(m_Settings.mutationMax);
}

void GeneticAlgorithm::InitializePopulation(std::vector<NeuralNetwork*> initialPopulation)
{
	for (NeuralNetwork* nn : initialPopulation)
	{
		m_Individuals.push_back(GeneticAlgorithm::Individual(nn, 0.f));
	}
}

void GeneticAlgorithm::InitializePopulation(const NeuralNetwork& networkTemplate, int populationSize, float weightInitRange, float biasInitRange)
{
	for (int i{}; i < populationSize; ++i)
	{
		NeuralNetwork* nn = new NeuralNetwork(networkTemplate);
		nn->InitWeightsRandom(-std::abs(weightInitRange), std::abs(weightInitRange));
		nn->InitBiasesRandom(-std::abs(biasInitRange), std::abs(biasInitRange));
		m_Individuals.push_back(GeneticAlgorithm::Individual(nn, 0.f));
	}

}

void GeneticAlgorithm::Run()
{
	for (int generationCounter{}; generationCounter < m_Settings.maxGenerations; ++generationCounter)
	{

		EvaluateFitness();

		//select partents
		std::vector<std::pair<NeuralNetwork*, NeuralNetwork*>> parents = SelectParents();

		//perform Crossover with selected parents
		for (int i{}; i < parents.size(); ++i)
		{
			auto child = Crossover(parents[i]);
		}

		//mutate the new population
		for (auto individual : m_Individuals)
		{
			Mutate(individual.pNetwork);
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
		individual.fitness = 0.f;
		individual.weight = 0.f;
	}
}

void GeneticAlgorithm::EvaluateFitness()
{
	ResetFitness();

	//create matches, first is white, black is second
	std::vector<std::pair<NeuralNetwork*, NeuralNetwork*>> gamePairings;

	for (int playerIndex{}; playerIndex < m_Individuals.size(); playerIndex++)
	{
		for (int gameNr{}; gameNr < m_Settings.gamesPlayed;)
		{
			int opponentIndex = rand() % m_Individuals.size();

			if (playerIndex != opponentIndex)
			{
				Individual player{ m_Individuals[playerIndex] };
				Individual opponent{ m_Individuals[opponentIndex] };
				auto gamePair = (gameNr < m_Settings.gamesPlayed / 2) ? std::pair(player.pNetwork, opponent.pNetwork) : std::pair(opponent.pNetwork, player.pNetwork);
				
				gamePairings.push_back(gamePair);

				//only increment the game nr if a match was successfully created
				gameNr++;
			}
		}
	}




	//play matches


	//process all match results

}

std::vector<std::pair<NeuralNetwork*, NeuralNetwork*>> GeneticAlgorithm::SelectParents()
{
	std::vector<std::pair<NeuralNetwork*, NeuralNetwork*>> parents{};

	float totalFitness{};
	for (const auto& ind : m_Individuals)
	{
		totalFitness += ind.fitness;
	}

	for (auto& ind : m_Individuals)
	{
		ind.weight = ind.fitness / totalFitness;
	}



	Individual parent1{};
	Individual parent2{};

	while (parents.size() < m_Individuals.size())
	{
		do
		{
			parent1 = PickIndividual();
			parent2 = PickIndividual();

		} while (parent1.pNetwork == parent2.pNetwork);

		//add the parents to the 
		parents.push_back(std::pair<NeuralNetwork*, NeuralNetwork*>(parent1.pNetwork, parent2.pNetwork));
	}

	return parents;
}

const GeneticAlgorithm::Individual& GeneticAlgorithm::PickIndividual()
{
	float currentValue{};

	std::random_device rd;
	std::uniform_real_distribution distr{ 0.f, 1.f };

	float targetValue{ distr(rd) };

	for (const auto& ind : m_Individuals)
	{
		currentValue += ind.weight;
		if (targetValue <= currentValue)
		{
			return ind;
		}
	}

	throw std::exception("something went wrong picking an individual");
}



NeuralNetwork GeneticAlgorithm::Crossover(std::pair<NeuralNetwork*, NeuralNetwork*> parents)
{
	//uniform crossover
	NeuralNetwork childNetwork = *parents.first;


	for (int index{}; index < childNetwork.GetNrLayerMatrices(); index++)
	{
		MatrixXf& matrix = childNetwork.GetLayerMatrix(index);

		for (int row{}; row < matrix.rows(); ++row)
		{
			for (int col{}; col < matrix.cols(); ++col)
			{
				matrix(row, col) = (rand() % 2 == 0) ? parents.first->GetLayerMatrix(index)(row, col) : parents.second->GetLayerMatrix(index)(row, col);
			}
		}
	}

	return childNetwork;
}


void GeneticAlgorithm::Mutate(NeuralNetwork* network)
{
	std::random_device rd;
	std::uniform_real_distribution uniDistr{ 0.f, 1.f };
	std::normal_distribution normalDistr{ 0.f, m_Settings.mutationDeviation };


	for (int index{}; index < network->GetNrLayerMatrices(); index++)
	{
		for (auto& value : network->GetLayerMatrix(index).reshaped())
		{
			float rand{ uniDistr(rd) };
			if (rand < m_Settings.mutationChance)
			{
				//mutate the gene (adding random generated value to the weight/bias)
				value += std::clamp(1 + normalDistr(rd), -m_Settings.mutationMax, m_Settings.mutationMax);
			}

		}
	}
}
