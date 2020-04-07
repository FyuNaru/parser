#include <iostream>

#include "../include/basis.h"

using namespace std;

Grammar::Grammar(ProductionList pList, SymbolSet tSet) : EPSILON("", Symbol::TERMINAL),
	END ("$", Symbol::TERMINAL), START(pList[0].first, Symbol::TERMINAL) {

	unordered_map<string, Symbol> strMap;
	for (const string& s : tSet) {
		Symbol non(s, Symbol::NON_TERMINAL);
		strMap.insert({s, non});
		_nonTerminals.push_back(non);
	}

	unordered_map<string, Symbol> nStrMap;
	int sum = 0;
	for (auto& pPair : pList) {
		Symbol head = strMap.at(pPair.first);
		int last = sum;
		for (auto& body : pPair.second) {
			vector<Symbol> bodyList;

			for (string& s : body) {
				if (strMap.count(s) == 1) {
					bodyList.push_back(strMap.at(s));
				}
				else {
					if (nStrMap.count(s) == 0) {
						 Symbol ter(s, Symbol::TERMINAL);
						 nStrMap[s] = ter;
						 _terminals.push_back(ter);
					}												 
					bodyList.push_back(nStrMap[s]);
				}
			}
			sum++;
			_productions.push_back({ head, bodyList });
		}
		_symbolRange.insert({ head, {last, sum} });
	}
}

string Grammar::toString() const {
	string ret;
	int i = -1;
	for (auto& p : _productions) {
		i++;
		ret += to_string(i) + "  ";
		ret += p.head.label();
		ret += " : ";
		for (const Symbol& s : p.body) {
			ret += s.label();
			ret += " ";
		}
		ret += "\n";
	}
	return ret;
}

Grammar::Table Grammar::firstTable() {
	Grammar::Table table;
	int now = 1;
	while (now > 0) {	
		now = 0;
		for (auto sPair : _symbolRange) {
			Symbol head = sPair.first;
			int last = table[head].size();
			int l = sPair.second.first, r = sPair.second.second;
			for (int i = l; i < r; ++i) {
				unordered_set<Symbol, HashSymbol> res = firstSet(_productions[i].body, table);
				table[head].insert(res.begin(), res.end());
				if (res.count(EPSILON) == 1)
					_nullableSet.insert(head);
			}
			now = now + table[head].size() - last;
		}
	}
	return table;
}

unordered_set<Symbol, HashSymbol> Grammar::firstSet(Grammar::Body subbody, Grammar::Table table) {
	unordered_set<Symbol, HashSymbol> res;
	int toEnd = 1;
	for (Symbol& b : subbody) {
		if (b.isTerminal()) {
			res.insert(b);
			if (b.label() != "")
				toEnd = 0;
			break;
		}
		else {
			unordered_set<Symbol, HashSymbol> elements = table[b];
			res.insert(elements.begin(), elements.end());
			if (_nullableSet.count(b) == 0) {
				toEnd = 0;
				break;
			}
		}
	}
	if (toEnd) {
		res.insert(EPSILON);
	}
	return res;
}

bool Grammar::isNullable(Symbol X) {
	return _nullableSet.count(X) == 1;
}

Grammar::Production Grammar::getProductionByKey(int key) {
	return _productions[key];
}

pair<int, int> Grammar::getProdutionRange(Symbol X) {
	if (_symbolRange.count(X) == 1)
		return _symbolRange[X];
	return { -1, -1 };
}

int HashSymbol::operator() (const Symbol& symbol) const {
	hash<string> hasher;
	return hasher(symbol.label());
}

bool operator== (const ItemKey& item1, const ItemKey& item2) {
	return item1.productionCode == item2.productionCode
		&& item1.location == item2.location;
}

int HashItemKey::operator() (const ItemKey& item) const {
	int r = 17;
	r = r << 5 - r + item.productionCode;
	r = r << 5 - r + item.location;
	return r;
}

bool operator== (const CItemKey& item1, const CItemKey& item2) {
	return item1.itemKey == item2.itemKey && item1.state == item2.state;
}

int HashCItemKey::operator() (const CItemKey& item) const {
	HashItemKey hik;
	int code = hik(item.itemKey);
	return code << 5 - code + item.state;
}