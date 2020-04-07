
#ifndef __ITEM_H__
#define __ITEM_H__

#include <iostream>
#include <stack>
#include <string>
#include <unordered_set>
#include <vector>
#include <cassert>

#include "basis.h"


/**
 * Equivalent to a LALR parser
 */
class PDA 
{
  public:

    /**
	 * Canonical LALR item set, i.e. , a LALR state
	 */
	class CanonicalItemSet 
	{
	  public:

		friend class HashCanonicalItemSet;
		friend class PDA;

		using ItemSet = std::unordered_set<ItemKey, HashItemKey>;
		using ItemMap = std::unordered_map<ItemKey, std::unordered_set<Symbol, HashSymbol>, HashItemKey>;

		CanonicalItemSet(ItemMap& kernels, Grammar& g, Grammar::Table& table);

		/**
		 * constructor: used for mapping
		 */
		CanonicalItemSet(ItemMap& kernels);

		bool operator== (const CanonicalItemSet& set1) const;

		ItemMap closure(ItemMap& items, Grammar& g, Grammar::Table& table);

	private:

		ItemSet _eq;

		/** items */
		ItemMap _kernels;		
		ItemMap _nonKernels;

		/** placeholders used for determine propagating relation */
		std::unordered_map<Symbol, ItemKey, HashSymbol> _fakeLookaheads;

		/** symbols next to a dot for each items */
		std::unordered_set<Symbol, HashSymbol> _nextSymbols;
	};

	/**
	 * Actions of an parsing entry
	 */
	enum ACTION 
	{ 
		SHIFT      = 0, 
		REDUCTION  = 1, 
		ACCEPT     = 2, 
		NONE       = 3
	};

	struct Entry 
	{
		ACTION action;
		/** {SHIFT, state number}, {REDUCTION, prodution number}, 
			{ACCEPT, -1},          {NONE, state number} 
		*/
		int actionCode;
	};

	using Table = std::vector<std::unordered_map<Symbol, Entry, HashSymbol>>;

	PDA(Grammar& grammar);

	std::string parsingTableHTML();

	bool parse(std::vector<std::string>& tokens);

  private:

	/** state set; canItems[state number] = state */
	std::vector<CanonicalItemSet> canItems;

	/** LALR parsing table */
	Table _parsingTable;

	Grammar& _grammar;

	// TODO
	std::unordered_map<CItemKey, std::vector<CItemKey>, HashCItemKey> ptable;
};

/**
 * Hash value: Canonical Item Set
 */
class HashCanonicalItemSet 
{
  public:

	int operator() (const PDA::CanonicalItemSet& itemSet) const;
};

#endif

