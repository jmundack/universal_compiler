#include "SymbolTable.h"
#include <algorithm>
#include <iostream>

using namespace std;

void SymbolTable::Begin()
{
   _ScopeNum++;
   _OpenScopes.push_back(_ScopeNum);
   cout << "Entered new scope " << endl;
}
void SymbolTable::End()
{
   if (!_Symbols[_OpenScopes.back()].empty())
   {
      const size_t pos = _Symbols[_OpenScopes.back()].at(0).first;
      _Data.erase(pos);
   }
   _OpenScopes.pop_back();
   cout << "Exited scope " << endl;
}

bool SymbolTable::Exists(const string &symbol)
{
   bool exists(false);
   for (size_t i = 0; i < _OpenScopes.size(); i++)
   {
      if (exists) break;
      const size_t scope(_OpenScopes.at(i));
      const ScopedSymbols &symbols(_Symbols[scope]);
      for (size_t j = 0; j < symbols.size(); j++)
      {
         const size_t first = symbols.at(j).first;
         const size_t second = symbols.at(j).second;
         string symbolInCache = _Data.substr(first,second);
         exists |= (symbol == symbolInCache);
         if (exists)
         {
            cout << "Symbol : " << symbol << " found in scope : " << i << endl;
            break;
         }
      }
   }
   return exists;
}

void SymbolTable::Add(const string &newSymbol)
{
   _Symbols[_ScopeNum].push_back(make_pair(_Data.size(), newSymbol.size()));
   _Data += newSymbol;
}

void SymbolTable::Print()
{
   cout << "Symbol Table : " << endl;
   cout << "--> Data : " << _Data << endl;
   for (size_t i = 0; i < _OpenScopes.size(); i++)
   {
      cout << "--> Scope : " << i << endl;
      cout << "    --> Symbols : " << endl;
      const ScopedSymbols &symbols(_Symbols[_OpenScopes.at(i)]);
      for (size_t j = 0; j < symbols.size(); j++)
         cout << "                 " << _Data.substr(symbols.at(j).first, symbols.at(j).second) << endl;
   }
}
