#include "TableGenerator.h"
#include <iostream>

using namespace std;

TableGenerator::TableGenerator(const string &filename):_Filename(filename), _PredictGenerator(filename){}

void TableGenerator::GenerateTable()
{
   const Productions &productions(_PredictGenerator.GetProductions());
   for (Productions::const_iterator itr = productions.begin(); itr != productions.end(); ++itr)
   {
      const string &lhs = itr->first;
      const vector<string> &rhs(itr->second);
      const size_t productionNum = itr - productions.begin() +1;
      for (vector<string>::const_iterator i = rhs.begin(); i != rhs.end(); ++i)
      {
         const set<string> &predictSet(_PredictGenerator.GetPredictSet(itr->first));
         if (predictSet.find(*i) != predictSet.end())
            _Table[make_pair(lhs,*i)] = productionNum;
      }
   }

   const Symbols &terminalSymbols = _PredictGenerator.GetTerminalSymbols();
   const Symbols &nonTerminalSymbols = _PredictGenerator.GetNonTerminalSymbols();
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
