#include "../include/item.h"

using namespace std;

using ItemMap = PDA::CanonicalItemSet::ItemMap;
using ItemSet = PDA::CanonicalItemSet::ItemSet;


PDA::CanonicalItemSet::CanonicalItemSet(ItemMap& kernels, Grammar& g, Grammar::Table& table) {

	for (auto& p : kernels) {
		_eq.insert(p.first);
		_kernels[p.first].insert(*(p.second.begin()));
		_fakeLookaheads.insert({ *(p.second.begin()), p.first });
		assert((p.second.size() == 1)); //TODO
	}
	ItemMap nk = closure(_kernels, g, table);
	_nonKernels.insert(nk.begin(), nk.end());
}

bool PDA::CanonicalItemSet::operator== (const CanonicalItemSet& set) const {
	return set._eq == _eq;
}

PDA::CanonicalItemSet::CanonicalItemSet(ItemMap& kernels) {
	for (auto& kf : kernels)
		_eq.insert(kf.first);
}


ItemMap PDA::CanonicalItemSet::closure(ItemMap& kernels, Grammar& g, Grammar::Table& table) {
	ItemMap map1(kernels.cbegin(), kernels.cend()), map2;
	ItemMap result;
	
	ItemMap& current = map1, & another = map2;
	int lastSum = 0;
	
	do {
		bool lookaheadUpdate = false;
		for (auto iter = current.cbegin(); iter != current.cend(); ++iter) {
			ItemKey itemKey = iter->first;
			auto lookaheads = iter->second;
			Grammar::Production p = g.getProductionByKey(itemKey.productionCode);
			if (itemKey.location == p.body.size()) 
				continue;
			_nextSymbols.insert(p.body[itemKey.location]);
			if (p.body[itemKey.location].isTerminal())
				continue;
			Symbol nextHead = p.body[itemKey.location];
			pair<int, int> range = g.getProdutionRange(nextHead);
			for (int i = range.first; i < range.second; ++i) {
				ItemKey t = { i, 0 };
				if (g.getProductionByKey(i).body[0].label() == "")
					t = { i, 1 };
				vector<Symbol> subbody(p.body.begin() + itemKey.location + 1, p.body.end());
				auto firstSet = g.firstSet(subbody, table);
				if (firstSet.count(g.EPSILON) == 1) {
					firstSet.erase(g.EPSILON);
					firstSet.insert(iter->second.begin(), iter->second.end());
				}
				another[t].insert(firstSet.begin(), firstSet.end());	
			}
		}
		for (auto& pairs : another) {
			ItemKey ikey = pairs.first;
			if (result.count(ikey) == 1) {
				for (const Symbol& l : pairs.second) {
					if (result[ikey].count(l) == 0) {
						result[ikey].insert(l);
						lookaheadUpdate = true;
					}
				}
			}
			else
				result.insert(pairs);
		}
		// result.insert(another.cbegin(), another.cend());
		
		int currentSum = result.size();
		if (currentSum == lastSum && !lookaheadUpdate)
			break;
		lastSum = currentSum;
		current.clear();

		ItemMap mid = current;	
		current = another;
		another = mid;
	} while (true);

	return result;
}


PDA::PDA(Grammar& grammar) : _grammar(grammar) {
	// record the propagating relation among kernel items
	unordered_map<CItemKey, vector<CItemKey>, HashCItemKey> propagationTable;
	// established state
	unordered_map<CanonicalItemSet, int, HashCanonicalItemSet> established;
	// state number
	int stateNum = 0;
	// first table of grammar G'
	Grammar::Table firstTable = grammar.firstTable();
	// special lookahead used for filling propagation table
	string t = "#" + to_string(stateNum) + "-" + to_string(0);
	// first production : S' -> S
	CanonicalItemSet::ItemMap startProduction = {{ {0, 0}, {Symbol(t, Symbol::TERMINAL)} } };
	CanonicalItemSet startState(startProduction, grammar, firstTable);

	// kernel items with spontaneous symbols
	unordered_map<CItemKey, unordered_set<Symbol, HashSymbol>, HashCItemKey> spontaneousItemSet;
	spontaneousItemSet.insert({ { {0, 0}, 0 }, {grammar.END} });
	
	// DFS initializations
	established.insert({ startState, 0 });
	stack<CanonicalItemSet> stateStack;
	stateStack.push(startState);
	canItems.push_back(startState);
	stateNum++;
	_parsingTable.push_back(unordered_map<Symbol, Entry, HashSymbol>());
	// complete following tasks during DFS:
	//   1. establish canonical LR(0) items collection; 2. fill propagation table
	//   3. fill GOTO entry and shifting ACTION entry of parsing table
	while (!stateStack.empty()) {
		auto topState = stateStack.top();
		stateStack.pop();
		for (const Symbol& symbol : topState._nextSymbols) {
			CanonicalItemSet::ItemMap nextStateKernels;

			// set up kernel items of next state by all its items
			for (auto& kp : topState._kernels) {
				ItemKey kpKey = kp.first;
				Grammar::Production production = grammar.getProductionByKey(kpKey.productionCode);
				if (kpKey.location != production.body.size() && production.body[kpKey.location] == symbol) {
					nextStateKernels.insert({ {kpKey.productionCode, kpKey.location + 1}, kp.second});
				}
			}
			for (auto& nkp : topState._nonKernels) {
				ItemKey nkpKey = nkp.first;
				Grammar::Production production = grammar.getProductionByKey(nkpKey.productionCode);
				if (nkpKey.location != production.body.size() && production.body[nkpKey.location] == symbol) {
					nextStateKernels.insert({ {nkpKey.productionCode, nkpKey.location + 1}, nkp.second });
				}
			}
			assert(nextStateKernels.size() > 0); //TODO

			
			// determine the relation of propagation
			CanonicalItemSet keying(nextStateKernels);
			int thisStateNum = stateNum;
			bool isOldState = (established.count(keying) == 1);
			if (isOldState)
				thisStateNum = established[keying];

			for (auto& kpair : nextStateKernels) {
				for (const Symbol& symbol : kpair.second) {
					if (topState._fakeLookaheads.count(symbol) == 1) {
						CItemKey cKey = { topState._fakeLookaheads[symbol], established.at(topState) };
						CItemKey cVal = { kpair.first, thisStateNum };
						propagationTable[cKey].push_back(cVal);
					}
					else {
						CItemKey cKey2 = { kpair.first, thisStateNum };
						spontaneousItemSet[cKey2].insert(symbol);
					}
				}
			}

			if (!isOldState) {
				int syNum = 0;
				// 1. assign new special lookaheads
				for (auto& kpair : nextStateKernels) {
					kpair.second.clear();
					string ts = "#" + to_string(stateNum) + "-" + to_string(syNum++);
					kpair.second.insert(Symbol(ts, Symbol::TERMINAL));
				}
				CanonicalItemSet nextState(nextStateKernels, grammar, firstTable);

				// 2. other
				stateStack.push(nextState);
				established.insert({ nextState, stateNum });
				stateNum++;
				canItems.push_back(nextState);
				_parsingTable.push_back(unordered_map<Symbol, Entry, HashSymbol>());
			}

			// fill part of parsing table
			int topStateIdx = established.at(topState);
			if (symbol.isTerminal()) {
				_parsingTable[topStateIdx][symbol] = { ACTION::SHIFT, thisStateNum };
			}
			else
				_parsingTable[topStateIdx][symbol] = { ACTION::NONE, thisStateNum };	
		}

		// eliminating lookaheads of top state kernels
		for (auto& kItem : canItems[established[topState]]._kernels) {
			kItem.second.clear();
		}
	}

	// propagating symbols among kernel items
	for (auto& cPair : spontaneousItemSet) { // iterate on source kernel (holds spontaneous symbol)
		auto cItem = cPair.first;
		auto proLookaheads = cPair.second;
		if (propagationTable.count(cItem) == 0) {
			canItems[cItem.state]._kernels[cItem.itemKey].insert(proLookaheads.cbegin(),
				proLookaheads.cend());
			continue;
		}
		

		stack<CItemKey> kStack;
		kStack.push(cItem);
		unordered_set<CItemKey, HashCItemKey> visited = { cItem };
		while (!kStack.empty()) {
			auto topCItem = kStack.top();
			kStack.pop();
			canItems[topCItem.state]._kernels[topCItem.itemKey].insert(proLookaheads.cbegin(),
				proLookaheads.cend());
			if (propagationTable.count(topCItem) == 0)
				continue;
			for (auto& nextCItem : propagationTable[topCItem]) {
				if (visited.count(nextCItem) == 1)
					continue;
				kStack.push(nextCItem);
			
				visited.insert(nextCItem);
			}
		}
	}

	for (int i = 0; i < canItems.size(); ++i) {
		// propagating symbols to non-kernel items
		for (auto& nkpair : canItems[i]._nonKernels) {
			auto oldLookaheads = nkpair.second;
			for (const Symbol& osym : oldLookaheads) {
				if (canItems[i]._fakeLookaheads.count(osym) == 1) {
					ItemKey& corrK = canItems[i]._fakeLookaheads[osym];
					nkpair.second.erase(osym);
					nkpair.second.insert(canItems[i]._kernels[corrK].begin(), 
						canItems[i]._kernels[corrK].end());
				}
			}
			// for EPSILON productions, they are nonKernel items, but they represent reduction
			if (grammar.getProductionByKey(nkpair.first.productionCode).body[0] == grammar.EPSILON) {
				for (const Symbol& emps : nkpair.second) {
					// ambiguity of "else" matching
					if (_parsingTable[i].count(emps) != 0 && emps.label() == "else") {
						continue;
					}
					// check conflicts
					assert(_parsingTable[i].count(emps) != 0);
					_parsingTable[i][emps] = { ACTION::REDUCTION, nkpair.first.productionCode };
				}
			}
		}
		// fill the reduction item and accept item of parsing table
		for (auto& kpair : canItems[i]._kernels) {
			Grammar::Production rP = grammar.getProductionByKey(kpair.first.productionCode);
			// Accept
			if (rP.head == grammar.START && kpair.first.location == 1) {
				assert(_parsingTable[i].count(grammar.END) == 0); //TODO
				_parsingTable[i][grammar.END] = { ACTION::ACCEPT, -1 };
				assert(rP.body.size() == 1); // TODO
			}
			// Reduction
			else if (kpair.first.location == rP.body.size()) {
				for (Symbol rSymbol : kpair.second) {		
					// check conflicts
					assert(_parsingTable[i].count(rSymbol) == 0); //TODO
					_parsingTable[i][rSymbol] = { ACTION::REDUCTION, kpair.first.productionCode };
				}
			}

		}
	}
	//TODO
	ptable = propagationTable; // using for debug
}

string PDA::parsingTableHTML() {
	Grammar& g = _grammar;
	string res;
	vector<Symbol> ters = g.terminals();
	ters.push_back(g.END);
	vector<Symbol> nters = g.nonTerminals();
	res += "<head><link rel=\"stylesheet\" href=\"T.css\"/></head><table class=\"lrgraph\"><tr>\n";
	res += "<th rowspan=\"2\" style=\"vertical-align:middle;\">State</th>\n";
	res += "<th colspan=\"" + to_string(ters.size()) + "\">ACTION</th>\n";
	res += "<th colspan=\"" + to_string(nters.size()) + "\">GOTO</th>\n</tr>\n<tr>";
	ters.insert(ters.end(), nters.begin(), nters.end());
	for (auto& symbol : ters) {
		res += "<th>";
		res += symbol.label();
		res += "</th>";
	}
	res += "</tr>";
	for (int i = 0; i < canItems.size(); ++i) {
		res += "<tr>\n<td>" + to_string(i) + "</td>";
		for (auto& symbol : ters) {
			res += "<td>";
			if (_parsingTable[i].count(symbol) == 1) {
				Entry& e = _parsingTable[i][symbol];
				string t;
				switch (e.action) {
					case SHIFT:
						t += "s" + to_string(e.actionCode);
						break;
					case REDUCTION:
						t += "r" + to_string(e.actionCode);
						break;
					case ACCEPT:
						t += "acc";
						break;
					case NONE:
						t += to_string(e.actionCode);
						break;
				}
				res += t;
			}
			res += "</td>";
		}		
		res += "</tr>\n";
	}
	return res;
}

bool PDA::parse(std::vector<string>& tokens) {
	vector<string> tks = tokens;
	if (tks[tks.size() - 1] != _grammar.END.label()) {
		tks.push_back(_grammar.END.label());
	}
	stack<Symbol> symStack;
	symStack.push(_grammar.END);
	stack<int> stateStack;
	stateStack.push(0);
	int i = 0;
	while (true) {
		int topState = stateStack.top();
		Symbol ts(tks[i], Symbol::TERMINAL);
		if (_parsingTable[topState].count(ts) == 0) { // TODO
			return false;
		}
		Entry entry = _parsingTable[topState][ts];
		if (entry.action == SHIFT) {
			symStack.push(ts);
			stateStack.push(entry.actionCode);
			i++;
		}
		else if (entry.action == REDUCTION) {
			Grammar::Production production = _grammar.getProductionByKey(entry.actionCode);

			cout << production.head.label() << " -> ";
			for (int j = 0; j < production.body.size(); ++j)
				cout << production.body[j].label();
			cout << endl;

			int num = production.body.size();
			// empty production
			if (production.body[0].label() == "")
				num = 0;
			for (int i = 0; i < num; ++i) {
				symStack.pop();
				stateStack.pop();
			}
			int currS = stateStack.top();
			symStack.push(production.head);
			if (_parsingTable[currS].count(production.head) == 0)
				return false;
			stateStack.push(_parsingTable[currS][production.head].actionCode);
		}
		else if (entry.action == ACCEPT) {
			return true;
		}
		else {
			return false;
		}
	}
}


int HashCanonicalItemSet::operator() (const PDA::CanonicalItemSet& itemSet) const {
	int r = 0;
	for (auto& p : itemSet._kernels) {
		ItemKey ik = p.first;
		HashItemKey hik;
		r += hik(p.first);
		r = r << 5 - r;
	}
	return r;
}



