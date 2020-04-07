#ifndef __DEF_H__
#define __DEF_H__

#include <string>
#include <unordered_set>
#include <vector>

/**
 * Grammar definitions
 */

using ProductionList = std::vector<std::pair<std::string, std::vector<std::vector<std::string>>>>;

extern ProductionList cProductions;

using SymbolSet = std::unordered_set<std::string>;

extern SymbolSet cNonTerminals;

extern ProductionList c1;

extern SymbolSet c2;

extern ProductionList cc1;

extern SymbolSet cc2;

#endif

