#ifndef __PARSERLOGGER_H__
#define __PARSERLOGGER_H__

#include <cstdio>

#define private public

#include "../../parser/include/basis.h"
#include "../../parser/include/item.h"

#undef private

using ItemMap = PDA::CanonicalItemSet::ItemMap;

class ParserLogger
{
  public:

      ParserLogger(const char* filename, Grammar& g);

      void printStateSet(PDA& pda);

      void printItemMap(ItemMap& itemMap);

      void printParsingTable(PDA& pda, const char* htmlFilename);

      void close();


  private:

    FILE* logFile;

    Grammar grammar;
};


#endif
