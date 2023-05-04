#include "GeneticAlgorithm.h"
#include <numeric>
#include <iostream>
#include <random>
#include "MiniMaxPlayer.h" 
#include "ChessGame.h"
#include "Timer.h"
#include <thread>
#include <future>
#include <algorithm>
#include <fstream>
#include <filesystem>

const std::string GeneticAlgorithm::m_ResultRootDir = "GA-Output";

GeneticAlgorithm::GeneticAlgorithm(const GeneticSettings& settings)
	:m_Settings{ settings }
	, m_GenerationCounter{ 0 }
{
	srand(unsigned int(time(NULL)));
	//if the mutation max setting is negative, turn it positive
	m_Settings.mutationMax = std::abs(m_Settings.mutationMax);
}

GeneticAlgorithm::GeneticAlgorithm(std::ifstream& settingsFile)
	: GeneticAlgorithm(LoadGeneticSettings(settingsFile))
{
}

void GeneticAlgorithm::InitializePopulationFromFile(std::ifstream& inputFile)
{
	if (inputFile)
	{
		LoadGeneration(inputFile);
	}
	else
	{
		throw std::exception("input vector is not valid");
	}
}

void GeneticAlgorithm::InitializeExistingPopulation(const std::vector<NeuralNetwork>& initialPopulation)
{
	int id{};
	for (const NeuralNetwork& nn : initialPopulation)
	{
		m_Individuals.push_back(std::make_shared<GeneticAlgorithm::Individual>(GeneticAlgorithm::Individual(std::make_unique<NeuralNetwork>(nn), 0.f, 0.f, id++)));
	}
}

void GeneticAlgorithm::InitializeNewPopulation(const NeuralNetwork& networkTemplate, int populationSize, float weightInitRange, float biasInitRange)
{
	for (int i{}; i < populationSize; ++i)
	{
		NeuralNetwork nn = networkTemplate;
		nn.InitWeightsRandom(-std::abs(weightInitRange), std::abs(weightInitRange));
		nn.InitBiasesRandom(-std::abs(biasInitRange), std::abs(biasInitRange));
		m_Individuals.push_back(std::make_shared<GeneticAlgorithm::Individual>(GeneticAlgorithm::Individual(std::make_unique<NeuralNetwork>(nn), 0.f, 0.f, i)));
	}

}

void GeneticAlgorithm::Run()
{
	PrepOutputFolder();
	SaveGeneticSettings();

	float totalTime{};
	Timer timer{};
	for (m_GenerationCounter = 0; m_GenerationCounter < m_Settings.maxGenerations; ++m_GenerationCounter)
	{
		std::cout << "Generation " << m_GenerationCounter + 1 << '/' << m_Settings.maxGenerations << " : " << std::endl;

		timer.Start();
		EvaluateFitness();
		std::cout << "Fitness evaluation time: " << timer.GetDuration<std::milli>() << "ms" << std::endl;
		totalTime += timer.GetDuration<std::milli>();

		auto greaterIndividual = [](const IndividualPtr& first, const IndividualPtr& second)
		{
			return (first->fitness > second->fitness);
		};

		//sort individuals to get the best in front
		std::partial_sort(m_Individuals.begin(), m_Individuals.begin() + m_Settings.elitismSize, m_Individuals.end(), greaterIndividual);

		//select partents
		std::vector<std::pair<IndividualPtr, IndividualPtr>> parents = SelectParents();

		for (int i{ m_Settings.elitismSize }; i < m_Individuals.size(); ++i)
		{
			int parentIndex = i - m_Settings.elitismSize;
			//create new child through crossover
			auto child = Crossover(parents[parentIndex].first->pNetwork.get(), parents[parentIndex].second->pNetwork.get());
			//mutate the values of the child
			Mutate(child);

			m_Individuals[i]->pNetwork = std::make_unique<NeuralNetwork>(child);
		}

		//reset fitness values
		ResetFitness();

		if (m_Settings.saveFrequency != 0 && m_GenerationCounter % m_Settings.saveFrequency == 0)
		{
			SaveGeneration();
			std::cout << "SAVED" << std::endl;
		}
		std::cout << "DONE" << std::endl << std::endl;;
	}

	SaveGeneration();
	std::cout << "Avg fitness time (" << m_Settings.threads << " threads): " << totalTime / m_Settings.maxGenerations << " ms" << std::endl;
}

void GeneticAlgorithm::SaveGeneration()
{

	std::stringstream ss{};
	ss << m_OutputPath << "/Generation-" << m_GenerationCounter << ".txt";
	std::ofstream out{ ss.str() };

	SaveGeneration(out);
	out.close();
}

void GeneticAlgorithm::PrepOutputFolder()
{
	std::stringstream ss{};
	ss << m_ResultRootDir << '/' << m_Settings.PopulationName;
	std::string path = ss.str();
	std::filesystem::remove_all(path);
	std::filesystem::create_directories(path);

	m_OutputPath = path;
}

void GeneticAlgorithm::SaveGeneticSettings()
{
	std::stringstream ss{};
	ss << m_OutputPath << "/GA-Settings.txt";
	std::ofstream out{ ss.str() };

	out << m_Settings.PopulationName << ' '
		<< m_Settings.maxGenerations << ' '
		<< m_Settings.threads << ' '
		<< m_Settings.saveFrequency << ' '
		<< m_Settings.gamesPlayed << ' '
		<< m_Settings.minMaxDepth << ' '
		<< m_Settings.elitismSize << ' '
		<< m_Settings.mutationChance << ' '
		<< m_Settings.mutationDeviation << ' '
		<< m_Settings.mutationMax << ' ';
}

GeneticSettings GeneticAlgorithm::LoadGeneticSettings(std::ifstream& settingsFile)
{
	if (settingsFile)
	{

		GeneticSettings settings{};

		settingsFile >> settings.PopulationName;
		settingsFile >> settings.maxGenerations;
		settingsFile >> settings.threads;
		settingsFile >> settings.saveFrequency;
		settingsFile >> settings.gamesPlayed;
		settingsFile >> settings.minMaxDepth;
		settingsFile >> settings.elitismSize;
		settingsFile >> settings.mutationChance;
		settingsFile >> settings.mutationDeviation;
		settingsFile >> settings.mutationMax;

		return settings;
	}
	else
	{
		throw std::exception("input file does not exist");
	}
}

void GeneticAlgorithm::SaveGeneration(std::ostream& out)
{
	out << m_Individuals.size() << std::endl;

	for (IndividualPtr ind : m_Individuals)
	{
		ind->pNetwork->Save(out);
	}
}

void GeneticAlgorithm::LoadGeneration(std::istream& in)
{
	std::string line{};
	std::getline(in, line);

	std::stringstream ss{ line };
	int popSize{};
	ss >> popSize;


	std::vector<NeuralNetwork> networks{};
	for (int i{}; i < popSize; i++)
	{
		networks.push_back(NeuralNetwork::Load(in));
	}

	InitializeExistingPopulation(networks);

}

void GeneticAlgorithm::ResetFitness()
{
	for (auto& individual : m_Individuals)
	{
		individual->fitness = 0.f;
		individual->weight = 0.f;
		individual->wins = 0;
		individual->draws = 0;
		individual->losses = 0;
	}
}

GeneticAlgorithm::GameRecord GeneticAlgorithm::PlayGame(IndividualPtr pWhite, IndividualPtr pBlack)
{
	GameRecord record{};
	record.pWhite = pWhite;
	record.pBlack = pBlack;


	MiniMaxPlayer<NNEval> wPlayer{ m_Settings.minMaxDepth, NNEval(pWhite->pNetwork.get()) };
	MiniMaxPlayer<NNEval> bPlayer{ m_Settings.minMaxDepth, NNEval(pBlack->pNetwork.get()) };

	ChessGame game = ChessGame(&wPlayer, &bPlayer, false);
	game.PlayGame();

	record.result = game.GetGameResult();
	return record;
}

std::vector<GeneticAlgorithm::GameRecord> GeneticAlgorithm::ProcessGames(const std::vector<std::pair<std::shared_ptr<Individual>, std::shared_ptr<Individual>>>& pairings)
{
	std::vector<GameRecord> records{};

	for (const auto& pairing : pairings)
	{
		records.push_back(PlayGame(pairing.first, pairing.second));
	}

	return records;
}

void GeneticAlgorithm::EvaluateFitness()
{
	ResetFitness();

	//create matches, first is white, black is second
	std::vector< std::vector<std::pair<std::shared_ptr<Individual>, std::shared_ptr<Individual>>>> gamePairings(m_Settings.threads);

	//shuffle individuals
	std::random_device rd{};
	auto rng = std::default_random_engine{ rd() };
	std::shuffle(m_Individuals.begin(), m_Individuals.end(), rng);


	for (int playerIndex{}; playerIndex < m_Individuals.size(); playerIndex++)
	{
		for (int gameNr{}; gameNr < m_Settings.gamesPlayed; gameNr++)
		{
			int gameIndex = gameNr + (playerIndex * m_Settings.gamesPlayed);

			int opponentIndex = (playerIndex + (gameNr + 1)) % m_Individuals.size();

			//create pair
			std::shared_ptr<Individual> player{ m_Individuals[playerIndex] };
			std::shared_ptr<Individual>  opponent{ m_Individuals[opponentIndex] };
			auto gamePair = (gameNr < m_Settings.gamesPlayed / 2) ?
				std::pair(std::shared_ptr<Individual>(player), std::shared_ptr<Individual>(opponent)) :
				std::pair(std::shared_ptr<Individual>(opponent), std::shared_ptr<Individual>(player));

			//put the pairings in separate lists according to how many threads are used
			gamePairings[gameIndex % m_Settings.threads].push_back(gamePair);
		}
	}


	std::vector<std::future<std::vector<GameRecord>>> futures{};

	//process the games asynchronously
	for (auto& pairingGroup : gamePairings)
	{
		futures.push_back(std::async(std::launch::async, &GeneticAlgorithm::ProcessGames, this, std::ref(pairingGroup)));
	}

	//loop over the futures and extract the gamerecords from them & process the results
	for (auto& future : futures)
	{
		for (const GameRecord& record : future.get())
		{
			switch (record.result)
			{
			case GameResult::Draw:
				record.pWhite->fitness += 0.5f;
				record.pBlack->fitness += 0.5f;
				record.pWhite->draws++;
				record.pBlack->draws++;
				break;
			case GameResult::WhiteWin:
				record.pWhite->fitness += 1.f;
				record.pWhite->wins++;
				record.pBlack->losses++;
				break;
			case GameResult::BlackWin:
				record.pBlack->fitness += 1.f;
				record.pWhite->losses++;
				record.pBlack->wins++;
				break;
			case GameResult::NoResult:
				assert(false && "it should be impossible for the game to have no result after playing!");
				break;
			default:
				break;
			}
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

	while (parents.size() < m_Individuals.size() - m_Settings.elitismSize)
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



NeuralNetwork GeneticAlgorithm::Crossover(NeuralNetwork* parent1, NeuralNetwork* parent2)
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
