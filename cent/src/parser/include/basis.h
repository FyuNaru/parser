#ifndef __BASIS_H__
#define __BASIS_H__

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "def.h"

/**
 * A Symbol of A Grammar.
 */
class Symbol 
{
	
  public:

	enum SymbolType { TERMINAL, NON_TERMINAL };

	Symbol() = default;

	Symbol(std::string label, SymbolType type) : _type(type), _label(label)  { }

	bool isTerminal() const { 
		return (_type == TERMINAL) ? true : false;
	}

	std::string label() const { return _label; }

	bool operator== (const Symbol& symbol) const {
		return _label == symbol.label();
	}

  private:

	SymbolType _type;

	std::string _label;
};


/**
 * hash value of Symbol
 */
class HashSymbol 
{
  public:
	int operator() (const Symbol& symbol) const;
};


/**
 * grammar
 */
class Grammar 
{
  public:

	using Body = std::vector<Symbol>;

	using Table = std::unordered_map<Symbol, std::unordered_set<Symbol, HashSymbol>, HashSymbol>;

	struct Production 
	{
		Symbol head;
		Body body;
	};

	Grammar(ProductionList pList, SymbolSet tSet);

	bool isNullable(Symbol X);

	std::pair<int, int> getProdutionRange(Symbol X);

	Production getProductionByKey(int key);

	std::string toString() const;

	/**
	 * @ return First table of this grammar
	 */
	Table firstTable();

	/**
	 * @param subbody A series of grammar symbols
	 * @param table First table of a grammar
	 *
	 * @return First set of sequence subbody
	 */
	std::unordered_set<Symbol, HashSymbol> firstSet(Body subbody, Table table);

	std::vector<Symbol> terminals() { return _terminals; }

	std::vector<Symbol> nonTerminals() { return _nonTerminals; }

	Symbol EPSILON;

	Symbol END;

	Symbol START;

  private:
	
	/** Index as a key of production */
	std::vector<Production> _productions;

	/** Head symbol corresponding to _productions [int1, int2) */
	std::unordered_map<Symbol, std::pair<int, int>, HashSymbol> _symbolRange;

	/** Symbols which can derive EPSILON */
	std::unordered_set<Symbol, HashSymbol> _nullableSet;

	std::vector<Symbol> _terminals;

	std::vector<Symbol> _nonTerminals;
};



/**
 * Identifying an LR(0) item
 */
struct ItemKey
{
	/** production key in a grammar */
	int productionCode;
	/** [0, production.body.size()] */
	int location;
};

bool operator== (const ItemKey& item1, const ItemKey& item2);

/**
 * Hash value: ItemKey
 */
class HashItemKey 
{
  public:

	int operator() (const ItemKey& item) const;
};



/**
 * Identifying a kernel item in a LALR constructing algorithm
 */
struct CItemKey 
{
	ItemKey itemKey;
	/** owner state (item collection) */
	int state;
};

bool operator== (const CItemKey& item1, const CItemKey& item2);

/**
 * Hash value: CItemKey
 */
class HashCItemKey 
{
  public:

	int operator() (const CItemKey& item) const;
};

#endif