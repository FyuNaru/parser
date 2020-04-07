#include "../include/def.h"

// set start production at ProductionList[0]
ProductionList c1 = {
	{ "P'"         ,  {{"Program"}}},
	{ "Program"    ,  {{"DecList"} }},
	{ "DecList"    ,  {{"DecList", "Dec"} , {"Dec"}}},
	{ "Dec"        ,  {{"VarDec"} , {"FuncDec"}}},
	 // support type
	{ "Type"       ,  {{"int"} , {"bool"},  {"double"}}},
	 // variable declaration
	{ "VarDec"     ,  {{"Type", "IdList", ";"}}},
	{ "IdList"     ,  {{"IdList", ",", "VarIni"} , {"VarIni"}}},
	{ "VarIni"     ,  {{"VarId"} , {"VarId", "=", "SimpleE"}}},
	{ "VarId"      ,  {{"id"} , {"id", "[", "const", "]"}}},
	 // function declaration
	{ "FuncDec"    ,  {{"Type", "id", "(", "Params", ")", "CompStmt"}}},
	{ "Params"     ,  {{"ParamList"} , {""}}},
	{ "ParamList"  ,  {{"ParamList", ",", "P"} , {"P"}}},
	{ "P"          ,  {{"Type", "ParamId"}}},
	{ "ParamId"    ,  {{"id"} , {"id", "[", "]"}}},
	 // statements
	{ "CompStmt"   ,  {{"{", "StmtList", "}"}}},
	{ "StmtList"   ,  {{"StmtList", "Stmt"} , {""}}},
	{ "Stmt"       ,  {{"WhileStmt"} , {"Expr", ";"}, {"IfStmt"}, {"CompStmt"}, {"VarDec"}, {";"}}},
	{ "IfStmt"     ,  {{"if", "(", "Expr", ")", "Stmt", "ElseP"}}},
	{ "ElseP"      ,  {{"else", "Stmt"} , {""}}},
	{ "WhileStmt"  ,  {{"while", "(", "Expr", ")", "Stmt"}}},
	// operators
	{ "Relop"     ,  {{"<"}, {"<="}, {"=="}, {"!="}, {">"}, {">="}}},
	{ "Sop"       ,  {{"+"}, {"-"}}},
	{ "Mop"       ,  {{"*"}, {"/"}, {"%"}}},
	{ "Uop"       ,  {{"-"},{ "*"}}},
	// expression
	{ "Expr"     ,  {{"Mut", "=", "Expr"} , {"SimpleE"}}},
	{ "Mut"      ,  {{"Mut", "[", "Expr", "]"} , {"id"}}},
	{ "SimpleE"  ,  {{"SimpleE", "or", "AndE"} , {"AndE"}}},
	{ "AndE"     ,  {{"AndE", "and", "NE"} , {"NE"}}},
	{ "NE"       ,  {{"not", "NE"} , {"RelE"}}},
	{ "RelE"     ,  {{"SumE", "Relop", "SumE"} , {"SumE"}}},
	{ "SumE"     ,  {{"SumE", "Sop", "MulE"} , {"MulE"}}},
	{ "MulE"     ,  {{"MulE", "Mop", "UE"} , {"UE"}}},
	{ "UE"       ,  {{"Uop", "UE"} , {"Factor"}}},
	{ "Factor"   ,  {{"IMut"} , {"Mut"}}},
	{ "IMut"     ,  {{"(", "Expr", ")"} , {"Call"}, {"const"}}},
	{ "Call"     ,  {{"id", "(", "Args", ")"}}},
	{ "Args"     ,  {{"ArgList"} , {""}}},
	{ "ArgList"  ,  {{"ArgList", ",", "Expr"} , {"Expr"}}},
};

SymbolSet c2 = {
	"P'", "Program", "DecList", "Dec", "VarDec", "FuncDec", "Type", "IdList", "VarIni", "VarId", "SimpleE", "Params", "CompStmt",
	"ParamList", "P", "ParamId", "StmtList", "Stmt", "WhileStmt", "Expr", "IfStmt", "ElseP", "Relop",
	"Sop", "Mop", "Mut", "AndE", "UE", "NE", "RelE", "Factor", "SumE", "MulE", "IMut", "Call", "Args", "ArgList", "Uop"
};