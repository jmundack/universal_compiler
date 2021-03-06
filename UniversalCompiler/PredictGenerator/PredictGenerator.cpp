#include "PredictGenerator.h"
#include <iostream>

using namespace std;

const pair<string,size_t> Lamda = make_pair("lamda", 0);
const bool Debug(false);

PredictGenerator::PredictGenerator(const string &filename):_Filename(filename), _Grammer(filename)
{
   _Grammer.Analyze();
   GeneratePredictSets();
}

void PredictGenerator::_MarkLamda()
{
   if(Debug) cout << "************ " << __FUNCTION__ << " *******************" << endl;
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
         {
            if (i->at(0) == '#') continue; // skip action symbols
            rhsDerivesLamda &= _DerivesLamda[*i];
         }
         if (rhsDerivesLamda && !_DerivesLamda[itr->first])
         {
            changes = true;
            _DerivesLamda[itr->first] = true;
         }
      }
   }

   for (map<string, bool>::const_iterator itr = _DerivesLamda.begin(); itr != _DerivesLamda.end(); ++itr)
      cout << " DerivesLamda[" << itr->first << "] is " << (itr->second ? "true" : "false") << endl;
   if(Debug) cout << "************ DONE " << __FUNCTION__ << " *******************" << endl;
}

SymbolProdNums PredictGenerator::_ComputeFirst(const vector<string> &symbols)
{
   if(Debug) 
   {
      cout << "************ " << __FUNCTION__ << " *******************" << endl;
      cout << "Symbols : ";
      for (vector<string>::const_iterator itr = symbols.begin(); itr != symbols.end(); ++itr)
         cout << *itr << "   ";
      cout << endl;
   }
   SymbolProdNums first;
   size_t i = 0;
   while (i < symbols.size())
   {
      if (symbols.at(i).at(0) != '#')
      {
         const SymbolProdNums &firstSet(_FirstSet[symbols.at(i)]);
         first.insert(firstSet.begin(), firstSet.end());
         if (firstSet.find(Lamda) == firstSet.end())
            break;
         first.erase(Lamda);
      }
      i++;
   }
   if (i == symbols.size())
   {
      const set<pair<string, size_t> > &firstSet(_FirstSet[symbols.at(i-1)]);
      if (firstSet.find(Lamda) != firstSet.end())
         first.insert(Lamda);
   }
   if(Debug) 
   {
      cout << "First is : ";
      for (SymbolProdNums::const_iterator itr = first.begin(); itr != first.end(); ++itr)
         cout << itr->first << "(" << itr->second << ")   ";
      cout << endl;
      cout << "************ DONE " << __FUNCTION__ << " *******************" << endl;
   }
   return first;
}

void PredictGenerator::_FillFirstSet()
{
   if (Debug) cout << "************ " << __FUNCTION__ << " *******************" << endl;

   const Symbols &nonTerminalSymbols(_Grammer.GetNonTerminalSymbols());
   for (Symbols::const_iterator itr = nonTerminalSymbols.begin(); itr != nonTerminalSymbols.end(); ++itr)
   {
      if (_DerivesLamda[*itr])
         _FirstSet[*itr].insert(Lamda);
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
         for (vector<string>::const_iterator rhsItem = j->second.begin(); rhsItem != j->second.end(); ++rhsItem)
         {
            if (rhsItem->at(0) == '#') continue; // skip action symbols
            if (*rhsItem == *itr)
            {
               _FirstSet[j->first].insert(make_pair(*itr, j - productions.begin() + 1));
            }
            break;
         }
      }
   }

   bool changes(true);
   while(changes)
   {
      changes = false;
      for (Productions::const_iterator j = productions.begin(); j != productions.end(); ++j)
      {
         const size_t firstSetSize = _FirstSet[j->first].size();
         const SymbolProdNums &computerFirst(_ComputeFirst((*j).second));
         for (SymbolProdNums::const_iterator itr = computerFirst.begin(); itr != computerFirst.end(); ++itr)
            _FirstSet[(*j).first].insert(make_pair(itr->first, j - productions.begin() +1));
         changes |= (firstSetSize != _FirstSet[j->first].size());
      }
   }
   if(Debug) cout << "************ DONE " << __FUNCTION__ << " *******************" << endl;
}

void PredictGenerator::_FillFollowSet()
{
   if(Debug) cout << "************ " << __FUNCTION__ << " *******************" << endl;

   const Productions &productions(_Grammer.GetProductions());
   bool changes(true);
   while(changes)
   {
      changes = false;
      for (Productions::const_iterator j = productions.begin(); j != productions.end(); ++j)
      {
         const string &lhs(j->first);
         const vector<string> &rhsWithActionSymbols(j->second);
         vector<string> rhs;
         for (vector<string>::const_iterator i = rhsWithActionSymbols.begin(); i != rhsWithActionSymbols.end(); ++i)
         {
            if (i->at(0) != '#')
               rhs.push_back(*i);
         }
         for (vector<string>::const_iterator i = rhs.begin(); i != rhs.end(); ++i)
         {
            const size_t followSetSize(_FollowSet[*i].size());
            if (i->at(0) == '<' && (i+1) != rhs.end())
            {
               const set<pair<string, size_t> > &first(_FirstSet[*(i+1)]);
               for (SymbolProdNums::const_iterator itr = first.begin(); itr != first.end(); ++itr)
               {
                  if (Lamda.first == itr->first)
                     _FollowSet[*i].insert(_FollowSet[lhs].begin(), _FollowSet[lhs].end());
                  else
                  {
                     if (itr->second == 0)
                        _FollowSet[*i].insert(make_pair(itr->first, j- productions.begin() +1));
                     else
                        _FollowSet[*i].insert(*itr);
                  }
               }
            }
            else if (i+1 == rhs.end())
               _FollowSet[*i].insert(_FollowSet[lhs].begin(), _FollowSet[lhs].end());
            changes |= (followSetSize != _FollowSet[*i].size());
         }
      }
   }
   if(Debug) cout << "************ DONE " << __FUNCTION__ << " *******************" << endl;
}

void PredictGenerator::GeneratePredictSets()
{
   _DerivesLamda.clear();
   _FirstSet.clear();
   _FollowSet.clear();
   _PredictSets.clear();
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

void PredictGenerator::Print()
{
   cout << "******************************************************************" << endl;
   cout << "First Sets:" << endl;
   for (SymbolSet::const_iterator itr = _FirstSet.begin(); itr != _FirstSet.end(); ++itr)
   {
      const SymbolProdNums &symbolProdNum(itr->second);
      cout << "First Set for : " << itr->first << endl;
      for (SymbolProdNums::const_iterator i = symbolProdNum.begin(); i != symbolProdNum.end(); ++i)
      cout << "\t" << i->first << " from " << i->second << endl;
   }

   cout << "******************************************************************" << endl;
   cout << "Follow Sets:" << endl;
   for (SymbolSet::const_iterator itr = _FollowSet.begin(); itr != _FollowSet.end(); ++itr)
   {
      const SymbolProdNums &symbolProdNum(itr->second);
      cout << "Follow Set for : " << itr->first << endl;
      for (SymbolProdNums::const_iterator i = symbolProdNum.begin(); i != symbolProdNum.end(); ++i)
         cout << "\t" << i->first << " from " << i->second << endl;


   }
   cout << "******************************************************************" << endl;
   cout << "Predic sets : " << endl;
   for (PredictSets::const_iterator itr = _PredictSets.begin(); itr != _PredictSets.end(); ++itr)
   {
      const set<pair<string, size_t > > &firstSet(itr->second);
      cout << "Predict Set for : " << itr->first << endl;
      for (set<pair<string, size_t> >::const_iterator i = firstSet.begin(); i != firstSet.end(); ++i)
         if (i->second != 0)
            cout << "\t" << i->first << "(" << i->second << ")" << endl;
   }
}


