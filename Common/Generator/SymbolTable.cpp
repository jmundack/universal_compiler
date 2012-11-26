#include "SymbolTable.h"
#include <algorithm>

using namespace std;

bool SymbolTable::Exists(const string &symbol) const 
{
   return (find(_Symbols.begin(), _Symbols.end(), symbol) != _Symbols.end());
}

void SymbolTable::Add(const string &newSymbol)
{
   _Symbols.push_back(newSymbol);
}


