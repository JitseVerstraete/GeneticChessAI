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
const std::string GeneticAlgorithm::m_SettingsFileName = "GA-Settings.txt";
const std::string GeneticAlgorithm::m_GenerationFileName = "Generation-";
const std::string GeneticAlgorithm::m_FileExtension = ".txt";

GeneticAlgorithm::GeneticAlgorithm(const GeneticSettings& settings)
	:m_Settings{ settings }
	, m_GenerationCounter{ 0 }
{
	srand(unsigned int(time(NULL)));
	//if the mutation max setting is negative, turn it positive
	m_Settings.mutationMax = std::abs(m_Settings.mutationMax);
}

GeneticAlgorithm::GeneticAlgorithm(const std::string& generationName)
	: GeneticAlgorithm(LoadGeneticSettings(generationName))
{
}

void GeneticAlgorithm::InitializePopulationFromFile(const std::string& generationName, int generationNumber)
{
	std::stringstream ss{};
	ss << m_ResultRootDir << '/' << generationName << '/' << m_GenerationFileName << generationNumber << m_FileExtension;
	std::ifstream inputFile{ ss.str() };

	if (inputFile)
	{
		LoadGeneration(inputFile);
	}
	else
	{
		throw std::exception("file does not exist");
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
	m_GenerationCounter = 0;
	PrepOutputFolder();
	SaveGeneticSettings();
	SaveGeneration();

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

const std::vector<GeneticAlgorithm::IndividualPtr>& GeneticAlgorithm::GetIndividuals()
{
	return m_Individuals;
}

void GeneticAlgorithm::SaveGeneration()
{

	std::stringstream ss{};
	ss << m_ResultRootDir << '/' << m_Settings.PopulationName << '/' << m_GenerationFileName << m_GenerationCounter << m_FileExtension;
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


}



MatchResults GeneticAlgorithm::Compare(GeneticAlgorithm& other, int nrBestPlayers)
{
	enum class Team
	{
		Team1,
		Team2
	};

	MatchResults results{};


	//compare the X best players of both generations against each other
	EvaluateFitness();
	other.EvaluateFitness();

	auto greaterIndividual = [](const IndividualPtr& first, const IndividualPtr& second)
	{
		return (first->fitness > second->fitness);
	};

	//sort 
	std::partial_sort(m_Individuals.begin(), m_Individuals.begin() + nrBestPlayers, m_Individuals.end(), greaterIndividual);
	std::partial_sort(other.m_Individuals.begin(), other.m_Individuals.begin() + nrBestPlayers, other.m_Individuals.end(), greaterIndividual);

	std::vector<std::pair<IndividualPtr, IndividualPtr>> pairings{};

	//let every player from every play against every player of the other team with both white and black
	for (int thisInd{}; thisInd < nrBestPlayers; thisInd++)
	{
		for (int otherInd{}; otherInd < nrBestPlayers; otherInd++)
		{
			//let every pairing play with black and white.
			pairings.push_back({ m_Individuals[thisInd], other.m_Individuals[otherInd] });
			pairings.push_back({ other.m_Individuals[otherInd], m_Individuals[thisInd] });
		}
	}

	//play games
	std::vector<GameRecord> records = ProcessGames(pairings);

	ResetFitness();
	other.ResetFitness();


	//process the results
	for (const GameRecord& record : records)
	{
		switch (record.result)
		{
		case GameResult::Draw:
			record.pWhite->draws++;
			record.pBlack->draws++;
			break;
		case GameResult::WhiteWin:
			record.pWhite->wins++;
			record.pBlack->losses++;
			break;
		case GameResult::BlackWin:
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

	//tally up the results
	MatchResults cmpResults{};
	for (int i{}; i < nrBestPlayers; i++)
	{
		results.wins += m_Individuals[i]->wins;
		results.draws += m_Individuals[i]->draws;
		results.losses += m_Individuals[i]->losses;

		cmpResults.wins += other.m_Individuals[i]->wins;
		cmpResults.draws += other.m_Individuals[i]->draws;
		cmpResults.losses += other.m_Individuals[i]->losses;
	}


	std::cout << "this wins:" << results.wins << std::endl;
	std::cout << "this draws:" << results.draws << std::endl;
	std::cout << "this losses:" << results.losses << std::endl << std::endl;

	std::cout << "other wins:" << cmpResults.wins << std::endl;
	std::cout << "other draws:" << cmpResults.draws << std::endl;
	std::cout << "other losses:" << cmpResults.losses << std::endl << std::endl;

	std::cout << "wins match: " << std::boolalpha << (results.wins == cmpResults.losses) << std::endl;
	std::cout << "draws match: " << std::boolalpha << (results.draws == cmpResults.draws) << std::endl;
	std::cout << "losses match: " << std::boolalpha << (results.losses == cmpResults.wins) << std::endl << std::endl;


	return results;
}

void GeneticAlgorithm::SaveGeneticSettings()
{
	std::stringstream ss{};
	ss << m_ResultRootDir << '/' << m_Settings.PopulationName << '/' << m_SettingsFileName;
	std::ofstream out{ ss.str() };

	out << m_Settings.PopulationName << ' '
		<< m_Settings.maxGenerations << ' '
		<< m_Settings.threads << ' '
		<< m_Settings.saveFrequency << ' '
		<< m_Settings.gamesPlayed << ' '
		<< m_Settings.minMaxDepth << ' '
		<< m_Settings.ttSize << ' '
		<< m_Settings.elitismSize << ' '
		<< m_Settings.mutationChance << ' '
		<< m_Settings.mutationDeviation << ' '
		<< m_Settings.mutationMax << ' ';
	out.close();
}

GeneticSettings GeneticAlgorithm::LoadGeneticSettings(const std::string& GenerationName)
{
	std::stringstream ss{};
	ss << m_ResultRootDir << '/' << GenerationName << '/' << m_SettingsFileName;
	std::ifstream settingsFile{ ss.str() };

	if (settingsFile)
	{

		GeneticSettings settings{};

		settingsFile >> settings.PopulationName;
		settingsFile >> settings.maxGenerations;
		settingsFile >> settings.threads;
		settingsFile >> settings.saveFrequency;
		settingsFile >> settings.gamesPlayed;
		settingsFile >> settings.minMaxDepth;
		settingsFile >> settings.ttSize;
		settingsFile >> settings.elitismSize;
		settingsFile >> settings.mutationChance;
		settingsFile >> settings.mutationDeviation;
		settingsFile >> settings.mutationMax;

		settingsFile.close();

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

	//todo: make genetic setting that passes tt size to the players
	MiniMaxPlayer<NNEval> wPlayer{ m_Settings.minMaxDepth, NNEval(pWhite->pNetwork.get()), (size_t)m_Settings.ttSize };
	MiniMaxPlayer<NNEval> bPlayer{ m_Settings.minMaxDepth, NNEval(pBlack->pNetwork.get()), (size_t)m_Settings.ttSize };

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
