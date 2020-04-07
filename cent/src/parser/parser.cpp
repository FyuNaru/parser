#include <iostream>

#include "include/basis.h"
#include "include/item.h"

using namespace std;

vector<string> testCode = {
		"bool", "id", "[", "const", "]", "=", "const", ";",   // 全局数组变量
		"int", "id", "(", "int", "id", ",", "double", "id", "[", "]", ")", "{",                           // 函数定义
			"if", "(", "id", "==", "id", ")", "{",            // if else
				"while", "(", "*", "id", "/", "id", "<=", "const", ")", "{", // while
					"int", "id", "=", "const", ";",                 // 变量
					"id", "(", "const", ",", "id", "+", "id", ")", ";", // 函数调用
				"}",
			"}", "else", "{",
				"if", "(", "*", "id", "+", "id", "%", "(", "-", "id", ")", "<", "const", ")",
					"double", "id", "=", "id", "*", "id",";",
			"}",
			"int", "id", "[", "const", "]", ";",
			"id", "[", "const", "]", "=", "id", "[", "const", "]", ";", // 引用数组变量，并赋值给另一个数组变量
		"}"
};

int main() {
	// 构造文法
	Grammar grammar(c1, c2);
	cout << grammar.toString() << endl;
	// first 表
	auto i = grammar.firstTable();
	PDA pda(grammar);
	// 分析表的 HTML 字符串
	pda.parsingTableHTML();
	// 语法分析
	cout << "\nReductions: \n" << endl;
	bool result = pda.parse(testCode);
	cout << result << endl;
}


