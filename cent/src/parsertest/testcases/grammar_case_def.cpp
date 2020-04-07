#include "grammar_case_def.h"

ProductionList cSimpleGrammar1List = {
	{ "S"   ,  {{"(" , "L", ")"} , {"a"}}},
	{ "L"   ,  {{"S" , "B"}}},
	{ "B"   ,  {{"," , "S", "B"} , {""}}},
};

SymbolSet cSimpleGrammar1NTSet = {
	"S", "L", "B"
};

ProductionList cSimpleGrammar2List = {
	{ "E"   ,  {{"T" , "E'"}}},
	{ "E'"  ,  {{"+", "T" , "E'"} , {""}}},
	{ "T"   ,  {{"F" , "T'"}}},
	{ "T'"  ,  {{"*" , "F", "T'"} , {""}}},
	{ "F"   ,  {{"(", "E" , ")"} , {"id"}}},
};

SymbolSet cSimpleGrammar2NTSet = {
	"E", "E'", "T", "T'", "F"
};

ProductionList cLALR1List = {
	{ "S'" , {{"S"}}},
	{ "S"  , {{"L", "=", "R"} , {"R"}}},
	{ "L"  , {{"*", "R"}, {"id"}}},
	{ "R"  , {{"L"}}}
};

SymbolSet cLALR1Set = {
	"S'", "S", "L", "R"
};

ProductionList cLALR2List = {
	{ "S'" ,  {{"S"}}},
	{ "S"  ,  {{"a", "A", "d"}, {"b", "B", "d"}, {"a", "B", "e"}, {"b", "A", "e"}}},
	{ "A"  ,  {{"c"}}},
	{ "B"  ,  {{"c"}}}
};

SymbolSet cLALR2Set = {
	"S'", "S", "A", "B"
};

ProductionList cControlFlowList = {
	{ "S'" ,  {{"S"}}},
	{ "S"  ,  {{"if", "B", "then", "S"}, {"if", "B", "then", "S", "else", "S"}, {"while", "B", "do", "S"}}},
	{ "B"  ,  {{"true"}, {"false"}}},
};

SymbolSet cControlFlowSet = {
	"S'", "S", "B"
};


ProductionList c1L = {
	{ "P'"    ,  {{"P"}}},
	{ "P"     ,  {{"D"} , {"S"}}},
	{ "S"     ,  {{"S", "S"} , {"id", "=", "E", ";"} , {"L", "=", "E", ";"} , {"if", "B", "then", "S"},
				  {"if", "B", "then", "S1", "else", "S2"}, {"while", "B", "do", "S1"},
				  {"call", "id", "(", "Elist", ")"}}},
	{ "D"     ,  {{"D", "D"} , {"proc", "id", ";", "D", "S"} , {"T", "id", ";"}}},
	{ "T"     ,  {{"X", "C"} , {"record", "D"}}},
	{ "X"     ,  {{"integer"} , {"real"}}},
	{ "C"     ,  {{"[", "num", "]", "C"} , {""}}},

	{ "E"     ,  {{"E", "+", "E"} , {"E", "*", "E"} , {"-", "E"} , {"(", "E", ")"} , {"id"} , {"digit"} , {"L"}}},
	{ "L"     ,  {{"id", "[", "E", "]"} , {"L", "[", "E", "]"}}},
	{ "B"     ,  {{"B", "or", "B"} , {"B", "and", "B"} , {"not", "B"} , {"(", "B", ")"} , {"E", "relop", "E"},
				  {"true"} , {"false"}}},
	{ "relop"     ,  {{"<"}, {"<="}, {"=="}, {"!="}, {">"}, {">="}}},

	{ "Elist" ,  {{"Elist", "," , "E"}, {"E"}}},
	{ "S1" ,  {{"stmt"}}},
	{ "S2" ,  {{"stmt"}}},
};

SymbolSet c1P = {
	"P'", "P", "D", "S", "T", "X", "C", "E", "L", "B", "relop", "Elist", //"S1", "S2",
};

