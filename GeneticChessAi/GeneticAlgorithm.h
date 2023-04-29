#pragma once
#include "NeuralNetwork.h"
#include <vector>



struct GeneticSettings
{
	//general
	int maxGenerations = 100;
	//int saveFrequency = 100;

	//selection
	int gamesPlayed = 10;
	int minMaxDepth = 2;
	//int elitismSize = 0;

	//crossover

	//mutation
	float mutationChance = 0.1f;
	float mutationDeviation = 0.1f;
	float mutationMax = FLT_MAX;
};

class GeneticAlgorithm
{
	
public:
	GeneticAlgorithm(const GeneticSettings& settings);

	void InitializeExistingPopulation(const std::vector<NeuralNetwork>& initialPopulation);
	void InitializeNewPopulation(const NeuralNetwork& networkTemplate, int populationSize, float weightInitRange, float biasInitRange);

	void Run();

private:

	//structs
	struct Individual
	{
		std::unique_ptr<NeuralNetwork> pNetwork;
		float fitness;
		float weight;
	};
	using IndividualPtr = std::shared_ptr<Individual>;

	//variables
	GeneticSettings m_Settings;
	std::vector<std::shared_ptr<Individual>> m_Individuals;



	//methods
	void ResetFitness();
	void EvaluateFitness();
	NeuralNetwork Crossover(NeuralNetwork* parent1, NeuralNetwork* parent2);
	void Mutate(NeuralNetwork& network);

	std::vector<std::pair<IndividualPtr, IndividualPtr>> SelectParents();
	IndividualPtr PickIndividual();




};
