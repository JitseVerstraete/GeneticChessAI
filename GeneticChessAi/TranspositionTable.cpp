#include "TranspositionTable.h"
#include <random>
#include <time.h>
#include <iostream>
#include <bitset>



TranspositionTable::TranspositionTable(uint32_t size)
	:m_Size{ size }
{
	m_pHashTable = std::make_unique<std::vector<TTEntry>>(std::vector<TTEntry>(size));
}

TTEntry TranspositionTable::GetEntryAtHash(uint64_t hash)
{
	TTEntry entry = m_pHashTable->at(GetIndex(hash));
	if (entry.key == hash)
	{
		return entry;
	}

	//entry is not valid(not found)
	entry.valid = false;
	return entry;
}

void TranspositionTable::StoreEvaluation(uint64_t hash, float val, const thc::Move& move, int8_t depth, EntryType type)
{
	TTEntry newEntry{};
	newEntry.key = hash;
	newEntry.value = val;
	newEntry.move = move;
	newEntry.searchDepth = depth;
	newEntry.type = type;
	newEntry.valid = true;

	//todo: replacement strategy (dont just discard the old one)
	TTEntry& existingEntry = (*m_pHashTable)[GetIndex(hash)];

	//if the new hash is the same as the stored hash && the new hash has lower depth, dont add it
	if (existingEntry.key == hash)
	{
		if (newEntry.type != EntryType::Exact)
		{
			if (existingEntry.searchDepth > depth)
			{
				return;
			}
		}
	}

	existingEntry = newEntry;
}

void TranspositionTable::Clear()
{
	m_pHashTable = std::make_unique< std::vector<TTEntry>>(std::vector<TTEntry>(m_Size));
}

uint32_t TranspositionTable::GetIndex(uint64_t hash)
{
	return hash % m_Size;
}
