#include "TableGenerator.h"
#include <iostream>

using namespace std;

TableGenerator::TableGenerator(const string &filename):_Filename(filename), _PredictGenerator(filename)
{
   _PredictGenerator.GeneratePredictSets();
   GenerateTable();
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
         cout << "lhs : " << lhs << " pruduction num : " << i->second << endl;
         if (productions.at(i->second -1).first == lhs)
            _Table[make_pair(lhs,i->first)] = i->second;
         else // temp hack to get the correct production number for lamda based productions
         {
            cout << "Looking for lamda production for " << lhs << endl;
            for (Productions::const_iterator j = productions.begin(); j != productions.end(); ++j)
            {
               if (j->first == lhs && (j->second.at(0) == "lamda"))
               {
                  _Table[make_pair(lhs,i->first)] = j - productions.begin() + 1;
                  break;
               }
            }
         }
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
