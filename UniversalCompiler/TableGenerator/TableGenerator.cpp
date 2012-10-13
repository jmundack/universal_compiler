#include "TableGenerator.h"
#include <iostream>

using namespace std;

TableGenerator::TableGenerator(const string &filename):_Filename(filename), _PredictGenerator(filename)
{
   _PredictGenerator.GeneratePredictSets();
}

void TableGenerator::GenerateTable()
{
   const Symbols &terminalSymbols = _PredictGenerator.GetTerminalSymbols();
   const Symbols &nonTerminalSymbols = _PredictGenerator.GetNonTerminalSymbols();
   const Productions &productions(_PredictGenerator.GetProductions());
   for (Productions::const_iterator itr = productions.begin(); itr != productions.end(); ++itr)
   {
      const string &lhs = itr->first;
      const SymbolProdNums&predictSet(_PredictGenerator.GetPredictSet(lhs));
      for (SymbolProdNums::const_iterator i = predictSet.begin(); i != predictSet.end(); ++i)
      {
         _Table[make_pair(lhs,i->first)] = i->second;
      }
   }

   cout << "\t\t";
   for (Symbols::const_iterator itr = terminalSymbols.begin(); itr != terminalSymbols.end(); ++itr)
      cout << "\t" << *itr;
   cout << endl;
   for (Symbols::const_iterator itr = nonTerminalSymbols.begin(); itr != nonTerminalSymbols.end(); ++itr)
   {
      cout << *itr << "\t";
      for (Symbols::const_iterator i = terminalSymbols.begin(); i != terminalSymbols.end(); ++i)
      {
         size_t productionNum = _Table[make_pair(*itr,*i)];
         if (productionNum != 0) cout << productionNum;
         cout <<"\t";
      }
      cout << endl << endl;
   }
}
