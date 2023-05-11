#pragma once
#include "NeuralNetwork.h"
#include <vector>
#include "ChessGame.h"



struct GeneticSettings
{
	//general
	std::string PopulationName = "UNNAMED";

	int maxGenerations = 100;
	int threads = 1;
	int saveFrequency = 0;

	//selection
	int gamesPlayed = 10;
	int minMaxDepth = 2;
	int ttSize = 1'000'000;
	int elitismSize = 0;

	//crossover

	//mutation
	float mutationChance = 0.1f;
	float mutationDeviation = 0.1f;
	float mutationMax = FLT_MAX;
};

class GeneticAlgorithm
{
	//structs
	struct Individual
	{
		std::unique_ptr<NeuralNetwork> pNetwork;
		float fitness;
		float weight;
		int id;
		int wins = 0;
		int draws = 0;
		int losses = 0;
	};
	using IndividualPtr = std::shared_ptr<Individual>;

public:
	GeneticAlgorithm(const GeneticSettings& settings);
	GeneticAlgorithm(std::ifstream& settingsFile);

	void InitializePopulationFromFile(std::ifstream& inputFile);
	void InitializeExistingPopulation(const std::vector<NeuralNetwork>& initialPopulation);
	void InitializeNewPopulation(const NeuralNetwork& networkTemplate, int populationSize, float weightInitRange, float biasInitRange);

	void Run();

	const std::vector<IndividualPtr>& GetIndividuals();

	void SaveGeneration();
	void SaveGeneticSettings();
	void PrepOutputFolder();

private:



	//variables
	GeneticSettings m_Settings;
	std::vector<std::shared_ptr<Individual>> m_Individuals;

	int m_GenerationCounter;
	std::string m_OutputPath;

	static const std::string m_ResultRootDir;

	//methods
	void ResetFitness();

	struct GameRecord
	{
		IndividualPtr pWhite;
		IndividualPtr pBlack;
		GameResult result;
	};

	GameRecord PlayGame(IndividualPtr white, IndividualPtr black);
	std::vector<GameRecord> ProcessGames(const std::vector<std::pair<std::shared_ptr<Individual>, std::shared_ptr<Individual>>>& pairings);
	void EvaluateFitness();

	NeuralNetwork Crossover(NeuralNetwork* parent1, NeuralNetwork* parent2);
	void Mutate(NeuralNetwork& network);

	std::vector<std::pair<IndividualPtr, IndividualPtr>> SelectParents();
	IndividualPtr PickIndividual();



	GeneticSettings LoadGeneticSettings(std::ifstream& settingsFile);

	void SaveGeneration(std::ostream& out);
	void LoadGeneration(std::istream& in);


};
