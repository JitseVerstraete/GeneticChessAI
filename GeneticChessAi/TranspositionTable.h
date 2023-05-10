#pragma once
#include <thc.h>
#include <cstdint>
#include <unordered_map>
#include <map>
#include <memory>


//if a 

enum class EntryType : uint8_t
{
	//the stored value is exactly equal to this 
	Exact,
	/*
	the value of the record is this or lower:
	When a record is stored as upper bound, it means that the search on this position was aborted
	before it could look into all possible moves as a result of the calculated value being
	lower than alpha in that position.

	*/
	MinNode,

	/*
	the value of the record is this or higher:
	When a record is stored as lower bound, it means that the search on this position was aborted
	before it could look into all possible moves as a result of the calculated value being
	higher than beta in that position.
	*/
	MaxNode	//the value of the record is this or higher
};

struct TTEntry
{
	TTEntry()
		:key{ 0 },
		value{ 0 },
		move{},
		type{ EntryType::Exact },
		searchDepth{ 0 },
		valid{ false }
	{
	}

	uint64_t key;
	float value;
	thc::Move move;
	EntryType type;
	int8_t searchDepth;
	bool valid;
};



class TranspositionTable
{
public:
	TranspositionTable(uint32_t size);

	TTEntry GetEntryAtHash(uint64_t hash);
	void StoreEvaluation(uint64_t hash, float val, const thc::Move& move,  int8_t depth, EntryType type);

	void Clear();

private:
	//hash table
	std::unique_ptr<std::vector<TTEntry>> m_pHashTable = nullptr;
	uint32_t m_Size;

	uint32_t GetIndex(uint64_t hash);

};
