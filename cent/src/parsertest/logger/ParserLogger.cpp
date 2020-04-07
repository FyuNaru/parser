#include "ParserLogger.h"

ParserLogger::ParserLogger(const char* filename, Grammar& g) : grammar(g) {
	logFile = fopen(filename, "w");
	if (logFile == NULL) {
		exit(1);
	}
}

void ParserLogger::printStateSet(PDA& pda) {
	for (int i = 0; i < pda.canItems.size(); ++i) {
		auto& state = pda.canItems[i];
		fprintf(logFile, "=======================================\n");
		fprintf(logFile, "             State Number: %d\n", i);
		fprintf(logFile, "=======================================\n");
		fprintf(logFile, "[Kernel Items]\n");
		printItemMap(state._kernels);
		fprintf(logFile, "\n[Non-Kernel Items]\n");
		printItemMap(state._nonKernels);
	}
}

void ParserLogger::printItemMap(ItemMap& itemMap) {
	for (auto& pairs : itemMap) {
		auto& itemKey = pairs.first;
		Grammar::Production p = grammar.getProductionByKey(itemKey.productionCode);
		fprintf(logFile, "%s -> ", p.head.label().c_str());
		int i = 0;
		for (i = 0; i < p.body.size(); ++i) {
			if (i == pairs.first.location) {
				fprintf(logFile, "@ ");
			}
			fprintf(logFile, "%s ", p.body[i].label().c_str());
		}
		if (i == pairs.first.location) {
			fprintf(logFile, "@ ");
		}
		fprintf(logFile, "\n\t\t(");
		for (auto& symbols : pairs.second) {
			fprintf(logFile, "%s ", symbols.label().c_str());
		}
		fprintf(logFile, ")\n");
	}
}

void ParserLogger::printParsingTable(PDA& pda, const char* htmlFileName) {
	FILE* h = fopen(htmlFileName, "w");
	fprintf(h ,pda.parsingTableHTML().c_str());
	fclose(h);
}

void ParserLogger::close() {
	fclose(logFile);
}