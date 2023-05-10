#pragma once
#include <thc.h>
#include <cstdint>
#include <unordered_map>
#include <map>
#include <memory>



struct TTEntry
{
	TTEntry()
		:key{ 0 },
		value{ 0 },
		move{},
		searchDepth{ 0 },
		valid{ false }
	{
	}

	uint64_t key;
	float value;
	thc::Move move;

	int8_t searchDepth;
	bool valid;
};



class TranspositionTable
{
public:
	TranspositionTable(uint32_t size);

	TTEntry GetEntryAtHash(uint64_t hash);
	void StoreEvaluation(uint64_t hash, float val, const thc::Move& move,  int8_t depth);

	void Clear();

private:
	//hash table
	std::unique_ptr<std::vector<TTEntry>> m_pHashTable = nullptr;
	uint32_t m_Size;

	uint32_t GetIndex(uint64_t hash);

};
