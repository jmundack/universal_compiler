#pragma once

#include "GrammerAnalyzer.h"
#include <map>

typedef std::map<std::string, std::set<std::pair<std::string, size_t> > > PredictSets;
typedef std::set<std::pair<std::string, size_t> > SymbolProdNums;

class PredictGenerator
{
public:
   PredictGenerator(const std::string &filename);
   void Print();
   void GeneratePredictSets();
   const Productions &GetProductions() const { return _Grammer.GetProductions(); }
   const Symbols &GetNonTerminalSymbols() const { return _Grammer.GetNonTerminalSymbols(); }
   const Symbols &GetTerminalSymbols() const { return _Grammer.GetTerminalSymbols(); }
   const std::set<std::pair<std::string, size_t > > &GetPredictSet(const std::string &nonTerminal) { return _PredictSets[nonTerminal]; }
private:

   void _MarkLamda();
   SymbolProdNums _ComputeFirst(const std::vector<std::string> &symbols);
   void _FillFirstSet();
   void _FillFollowSet();

   std::string _Filename;
   GrammerAnalyzer _Grammer;
   std::map<std::string, bool> _DerivesLamda;

   typedef std::map<std::string, std::set<std::pair<std::string, size_t> > > SymbolSet;
   SymbolSet _FirstSet;
   SymbolSet _FollowSet;
   PredictSets _PredictSets;
};
