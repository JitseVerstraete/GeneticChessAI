#pragma once
#include "NeuralNetwork.h"
#include <vector>


struct GeneticSettings
{
	//general
	int gamesPlayed;
	int maxGenerations;
	int saveFrequency;

	//selection

	//crossover

	//mutation
	float mutationChance;
	float mutationDeviation;
	float mutationMax;
};

class GeneticAlgorithm
{
public:
	GeneticAlgorithm(const GeneticSettings& settings);

	void InitializePopulation(std::vector<NeuralNetwork*> initialPopulation);
	void InitializePopulation(const NeuralNetwork& networkTemplate, int populationSize, float weightInitRange, float biasInitRange);

	void Run();
	NeuralNetwork Crossover(std::pair<NeuralNetwork*, NeuralNetwork*> parents);

private:

	//structs
	struct Individual
	{
		NeuralNetwork* pNetwork;
		float fitness;
		float weight;
	};


	//variables
	GeneticSettings m_Settings;
	std::vector<Individual> m_Individuals;



	//methods
	void ResetFitness();
	void EvaluateFitness();

	std::vector<std::pair<NeuralNetwork*, NeuralNetwork*>> SelectParents();
	const Individual& PickIndividual();


	void Mutate(NeuralNetwork* network);


};
