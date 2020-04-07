#include "CppUnitTest.h"

#include <iostream>
#include <cstdio>
#include <unordered_set>
#include <string>

#define private public

#include "../parser/include/basis.h"
#include "../parser/include/item.h"
#include "testcases/grammar_case_def.h"
#include "logger/ParserLogger.h"

#undef private

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace BasisTest {

	TEST_CLASS(Grammar_Test) {

	public:

		TEST_METHOD(firstSet1) {
			Grammar g(cSimpleGrammar1List, cSimpleGrammar1NTSet);
			auto table = g.firstTable();
			for (auto i = table.begin(); i != table.end(); ++i) {
				string symbol = i->first.label();
				unordered_set<string> firstSymbols;
				for (auto& j : i->second) {
					firstSymbols.insert(j.label());
				}
				if (symbol == "S" || symbol == "L") {
					Assert::IsTrue(firstSymbols.count("(") == 1);
					Assert::IsTrue(firstSymbols.count("a") == 1);
					Assert::IsFalse(firstSymbols.count("epsilon") == 1);
				}
				else {
					Assert::IsTrue(firstSymbols.count(",") == 1);
					Assert::IsTrue(firstSymbols.count("") == 1);
				}
			}
		}

		TEST_METHOD(firstSet2) {
			Grammar g(cSimpleGrammar2List, cSimpleGrammar2NTSet);
			auto table = g.firstTable();

			for (auto i = table.begin(); i != table.end(); ++i) {
				string symbol = i->first.label();
				unordered_set<string> firstSymbols;
				for (auto& j : i->second) {
					firstSymbols.insert(j.label());
				}
				if (symbol == "E" || symbol == "T" || symbol == "F") {
					Assert::IsTrue(firstSymbols.count("(") == 1);
					Assert::IsTrue(firstSymbols.count("id") == 1);
					Assert::IsFalse(firstSymbols.count("") == 1);
				}
				else if (symbol == "E'") {
					Assert::IsTrue(firstSymbols.count("+") == 1);
					Assert::IsTrue(firstSymbols.count("") == 1);
				}
				else {
					Assert::IsTrue(firstSymbols.count("*") == 1);
					Assert::IsTrue(firstSymbols.count("") == 1);
				}
			}
		}
	};
}


namespace ItemTest {

	TEST_CLASS(ItemSetTest) {

	public:

		TEST_METHOD(constructor) {
			//Grammar g = Grammar(cControlFlowList, cControlFlowSet);
			Grammar g = Grammar(c1, c2);
			PDA pda(g);
			ParserLogger logger("../../log/log.txt", g);
			logger.printStateSet(pda);
			logger.printParsingTable(pda, "../../log/table.html");
			
// 			for (auto& pt : pda.ptable) {
// 				ItemKey ik = pt.first.itemKey;
// 				Grammar::Production p = g.getProductionByKey(ik.productionCode);
// 				Logger::WriteMessage(p.head.label().c_str());
// 				Logger::WriteMessage(" -> ");
// 				int i;
// 				for (i = 0; i < p.body.size(); ++i) {
// 					if (i == ik.location)
// 						Logger::WriteMessage("@ ");
// 					Logger::WriteMessage(p.body[i].label().c_str());
// 					Logger::WriteMessage(" ");
// 				}
// 				if (i == ik.location)
// 					Logger::WriteMessage("@ ");
// 				char n[20];
// 				sprintf_s(n, " state(%d)\n", pt.first.state);
// 				Logger::WriteMessage(n);
// 
// 				for (auto& pi : pt.second) {
// 					Logger::WriteMessage("\t");
// 					ItemKey iki = pi.itemKey;
// 					Grammar::Production pp = g.getProductionByKey(iki.productionCode);
// 					int i;
// 					Logger::WriteMessage(pp.head.label().c_str());
// 					Logger::WriteMessage(" -> ");
// 					for (i = 0; i < pp.body.size(); ++i) {
// 						if (i == iki.location)
// 							Logger::WriteMessage("@ ");
// 						Logger::WriteMessage(pp.body[i].label().c_str());
// 						Logger::WriteMessage(" ");
// 					}
// 					if (i == iki.location)
// 						Logger::WriteMessage("@ ");
// 					char nn[20];
// 					sprintf_s(nn, " state(%d)\n", pi.state);
// 					Logger::WriteMessage(nn);
// 				}
// 			}	
		}
	};
}
