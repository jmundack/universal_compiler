#include "PredictGenerator.h"
#include <iostream>

using namespace std;

PredictGenerator::PredictGenerator(const string &filename):_Filename(filename), _Grammer(filename)
{
   _Grammer.Analyze();
}

void PredictGenerator::Print()
{
   cout << "First Sets:" << endl;
   for (SymbolSet::const_iterator itr = _FirstSet.begin(); itr != _FirstSet.end(); ++itr)
   {
      const SymbolProdNums &symbolProdNum(itr->second);
      cout << itr->first << endl;
      for (SymbolProdNums::const_iterator i = symbolProdNum.begin(); i != symbolProdNum.end(); ++i)
      cout << "\t" << i->first << " from " << i->second << endl;


   }

   cout << "Follow Sets:" << endl;
   for (SymbolSet::const_iterator itr = _FollowSet.begin(); itr != _FollowSet.end(); ++itr)
   {
      const SymbolProdNums &symbolProdNum(itr->second);
      cout << itr->first << endl;
      for (SymbolProdNums::const_iterator i = symbolProdNum.begin(); i != symbolProdNum.end(); ++i)
      cout << "\t" << i->first << " from " << i->second << endl;


   }
   for (PredictSets::const_iterator itr = _PredictSets.begin(); itr != _PredictSets.end(); ++itr)
   {
      const set<pair<string, size_t > > &firstSet(itr->second);
      cout << "Predict Set for : " << itr->first << endl;
      for (set<pair<string, size_t> >::const_iterator i = firstSet.begin(); i != firstSet.end(); ++i)
         if (i->second != 0)
            cout << "\t" << i->first << "(" << i->second << ")" << endl;
   }
}

void PredictGenerator::GeneratePredictSets()
{
   _MarkLamda();
   _FillFirstSet();
   _FillFollowSet();
   for (SymbolSet::const_iterator itr = _FirstSet.begin(); itr != _FirstSet.end(); ++itr)
   {
      const set<pair<string, size_t> > &firstSet(itr->second);
      for (set<pair<string,size_t > >::const_iterator i = firstSet.begin(); i != firstSet.end(); ++i)
      {
         if ((*i).first == "lamda")
         {
            const set<pair<string, size_t> > &followSet(_FollowSet[itr->first]);
            for (set<pair<string, size_t > >::const_iterator j = followSet.begin(); j != followSet.end(); ++j)
               if (j->second)
                  _PredictSets[itr->first].insert(make_pair(j->first, j->second));
         }
         else
            if (i->second)
               _PredictSets[itr->first].insert(make_pair(i->first, i->second));
      }
   }
}

void PredictGenerator::_MarkLamda()
{
   cout << "************ " << __FUNCTION__ << " *******************" << endl;
   const Productions &productions(_Grammer.GetProductions());
   const Symbols &nonTerminalSymbols(_Grammer.GetNonTerminalSymbols());
   for (Symbols::const_iterator itr = nonTerminalSymbols.begin(); itr != nonTerminalSymbols.end(); ++itr)
      _DerivesLamda[*itr] = false;
   const Symbols &terminalSymbols(_Grammer.GetTerminalSymbols());
   for (Symbols::const_iterator itr = terminalSymbols.begin(); itr != terminalSymbols.end(); ++itr)
      _DerivesLamda[*itr] = false;
   _DerivesLamda["lamda"] = true;
   bool changes = true;
   while (changes)
   {
      changes = false;
      for (Productions::const_iterator itr = productions.begin(); itr != productions.end(); ++itr)
      {
         bool rhsDerivesLamda = true;
         for (vector<string>::const_iterator i = (*itr).second.begin(); i != (*itr).second.end(); ++i)
            rhsDerivesLamda &= _DerivesLamda[*i];
         if (rhsDerivesLamda && !_DerivesLamda[itr->first])
         {
            changes = true;
            _DerivesLamda[itr->first] = true;
         }
      }
   }

   for (map<string, bool>::const_iterator itr = _DerivesLamda.begin(); itr != _DerivesLamda.end(); ++itr)
      cout << " DerivesLamda[" << itr->first << "] is " << (itr->second ? "true" : "false") << endl;
   cout << "************ DONE " << __FUNCTION__ << " *******************" << endl;
}

SymbolProdNums PredictGenerator::_ComputeFirst(const vector<string> &symbols)
{
   cout << "************ " << __FUNCTION__ << " *******************" << endl;
   cout << "Symbols : ";
   for (vector<string>::const_iterator itr = symbols.begin(); itr != symbols.end(); ++itr)
      cout << *itr << "   ";
   cout << endl;
   SymbolProdNums first;
   if (symbols.empty())
      first.insert(make_pair("lamda", 0));
   else
   {
      size_t i = 0;
      while (i < symbols.size())
      {
         const SymbolProdNums &firstSet(_FirstSet[symbols.at(i)]);
         first.insert(firstSet.begin(), firstSet.end());
         if (firstSet.find(make_pair("lamda",0)) == firstSet.end())
            break;
         first.erase(make_pair("lamda",0));
         i++;
      }
      if (i == symbols.size())
      {
         const set<pair<string, size_t> > &firstSet(_FirstSet[symbols.at(i-1)]);
         if (firstSet.find(make_pair("lamda",0)) != firstSet.end())
            first.insert(make_pair("lamda",0));
      }
   }
   cout << "First is : ";
   for (SymbolProdNums::const_iterator itr = first.begin(); itr != first.end(); ++itr)
      cout << itr->first << "(" << itr->second << ")   ";
   cout << endl;
   cout << "************ DONE " << __FUNCTION__ << " *******************" << endl;
   return first;
}

void PredictGenerator::_FillFirstSet()
{
   cout << "************ " << __FUNCTION__ << " *******************" << endl;
   const Symbols &nonTerminalSymbols(_Grammer.GetNonTerminalSymbols());
   for (Symbols::const_iterator itr = nonTerminalSymbols.begin(); itr != nonTerminalSymbols.end(); ++itr)
   {
      if (_DerivesLamda[*itr])
         _FirstSet[*itr].insert(make_pair("lamda",0));
      else
         _FirstSet[*itr].clear();
   }

   const Productions &productions(_Grammer.GetProductions());
   const Symbols &terminalSymbols(_Grammer.GetTerminalSymbols());
   for (Symbols::const_iterator itr = terminalSymbols.begin(); itr != terminalSymbols.end(); ++itr)
      _FirstSet[*itr].insert(make_pair(*itr, 0));

   for (Productions::const_iterator j = productions.begin(); j != productions.end(); ++j)
   {
      for (Symbols::const_iterator itr = terminalSymbols.begin(); itr != terminalSymbols.end(); ++itr)
      {
         if ((*j).second.at(0) == *itr)
            _FirstSet[j->first].insert(make_pair(*itr, j - productions.begin() + 1));
      }
   }

   for (Productions::const_iterator j = productions.begin(); j != productions.end(); ++j)
   {
      const SymbolProdNums &computerFirst(_ComputeFirst((*j).second));
      for (SymbolProdNums::const_iterator itr = computerFirst.begin(); itr != computerFirst.end(); ++itr)
         _FirstSet[(*j).first].insert(make_pair(itr->first, j - productions.begin() +1));
//      _FirstSet[(*j).first].insert(computerFirst.begin(), computerFirst.end());
   }
   cout << "************ DONE " << __FUNCTION__ << " *******************" << endl;
}

void PredictGenerator::_FillFollowSet()
{
   cout << "************ " << __FUNCTION__ << " *******************" << endl;

   const Productions &productions(_Grammer.GetProductions());
   for (Productions::const_iterator j = productions.begin(); j != productions.end(); ++j)
   {
      for (vector<string>::const_iterator i = (*j).second.begin(); i != (*j).second.end(); ++i)
      {
         if (i->at(0) == '<' && (i+1) != (*j).second.end())
         {
            const set<pair<string, size_t> > &first(_FirstSet[*(i+1)]);
            _FollowSet[*i].insert(first.begin(), first.end());
            _FollowSet.erase("lamda");
            if (first.find(make_pair("lamda",0)) != first.end())
            {
               _FollowSet[*i].insert(_FollowSet[j->first].begin(), _FollowSet[j->first].end());
            }
         }
      }
   }
   cout << "************ DONE " << __FUNCTION__ << " *******************" << endl;
}
