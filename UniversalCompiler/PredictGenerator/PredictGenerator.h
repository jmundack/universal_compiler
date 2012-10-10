#pragma once

#include "GrammerAnalyzer.h"
#include <map>

typedef std::map<std::string, std::set<std::string> > PredictSets;

class PredictGenerator
{
public:
   PredictGenerator(const std::string &filename);
   void Print();
   void GeneratePredictSets();
   const Productions &GetProductions() const { return _Grammer.GetProductions(); }
   const Symbols &GetNonTerminalSymbols() const { return _Grammer.GetNonTerminalSymbols(); }
   const Symbols &GetTerminalSymbols() const { return _Grammer.GetTerminalSymbols(); }
   const std::set<std::string> &GetPredictSet(const std::string &nonTerminal) { return _PredictSets.at(nonTerminal); }
private:

   void _MarkLamda();
   Symbols _ComputeFirst(const std::vector<std::string> &symbols);
   void _FillFirstSet();
   void _FillFollowSet();

   std::string _Filename;
   GrammerAnalyzer _Grammer;
   std::map<std::string, bool> _DerivesLamda;
   std::map<std::string, std::set<std::string> > _FirstSet;
   std::map<std::string, std::set<std::string> > _FollowSet;
   PredictSets _PredictSets;
};
