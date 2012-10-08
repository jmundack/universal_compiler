#include "PredictGenerator.h"
#include <iostream>

using namespace std;

PredictGenerator::PredictGenerator(const string &filename):_Filename(filename), _Grammer(filename)
{
   _Grammer.Analyze();
}

void PredictGenerator::PrintPredictSet()
{
   _MarkLamda();
   _FillFirstSet();
   _FillFollowSet();
   for (map<std::string, std::set<std::string> >::const_iterator itr = _FirstSet.begin(); itr != _FirstSet.end(); ++itr)
   {
      const set<string> &firstSet(itr->second);
      cout << "Predict Set for : " << itr->first << endl;
      for (set<string>::const_iterator i = firstSet.begin(); i != firstSet.end(); ++i)
      {
         if (*i == "lamda")
         {
            const set<string> &followSet(_FollowSet[itr->first]);
            for (set<string>::const_iterator j = followSet.begin(); j != followSet.end(); ++j)
            {
               cout << "\t\t" <<  *j << endl;
            }
         }
         else
            cout << "\t" << *i << endl;
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

Symbols PredictGenerator::_ComputeFirst(const vector<string> &symbols)
{
   cout << "************ " << __FUNCTION__ << " *******************" << endl;
   cout << "Symbols : ";
   for (vector<string>::const_iterator itr = symbols.begin(); itr != symbols.end(); ++itr)
      cout << *itr << "   ";
   cout << endl;
   Symbols first;
   if (symbols.empty())
      first.insert("lamda");
   else
   {
      size_t i = 0;
      while (i < symbols.size())
      {
         const set<string> &firstSet(_FirstSet[symbols.at(i)]);
         first.insert(firstSet.begin(), firstSet.end());
         if (firstSet.find("lamda") == firstSet.end())
            break;
         first.erase("lamda");
         i++;
      }
      if (i == symbols.size())
      {
         const set<string> &firstSet(_FirstSet[symbols.at(i-1)]);
         if (firstSet.find("lamda") != firstSet.end())
            first.insert("lamda");
      }
   }
   cout << "First is : ";
   for (set<string>::const_iterator itr = first.begin(); itr != first.end(); ++itr)
      cout << *itr << "   ";
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
         _FirstSet[*itr].insert("lamda");
      else
         _FirstSet[*itr].clear();
   }

   const Productions &productions(_Grammer.GetProductions());
   const Symbols &terminalSymbols(_Grammer.GetTerminalSymbols());
   for (Symbols::const_iterator itr = terminalSymbols.begin(); itr != terminalSymbols.end(); ++itr)
   {
      _FirstSet[*itr].insert(*itr);
      for (Symbols::const_iterator i = nonTerminalSymbols.begin(); i != nonTerminalSymbols.end(); ++i)
      {
         for (Productions::const_iterator j = productions.begin(); j != productions.end(); ++j)
         {
            if (*i != j->first) continue;
            if ((*j).second.at(0) == *itr)
               _FirstSet[*i].insert(*itr);
         }
      }
   }

   for (Productions::const_iterator j = productions.begin(); j != productions.end(); ++j)
   {
      const set<string> &computerFirst(_ComputeFirst((*j).second));
      _FirstSet[(*j).first].insert(computerFirst.begin(), computerFirst.end());
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
            const Symbols &first(_FirstSet[*(i+1)]);
            _FollowSet[*i].insert(first.begin(), first.end());
            _FollowSet.erase("lamda");
            if (first.find("lamda") != first.end())
            {
               _FollowSet[*i].insert(_FollowSet[j->first].begin(), _FollowSet[j->first].end());
            }
         }
      }
   }
   cout << "************ DONE " << __FUNCTION__ << " *******************" << endl;
}
