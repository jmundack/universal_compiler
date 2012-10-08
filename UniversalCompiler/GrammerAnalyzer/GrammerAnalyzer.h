#pragma once

#include <string>
#include <vector>
#include <set>

   typedef std::vector<std::pair<std::string, std::vector<std::string> > > Productions;
   typedef std::set<std::string> Symbols;
class GrammerAnalyzer
{
public:
   GrammerAnalyzer(const std::string &filename);
   void Analyze();
   const Productions &GetProductions() const { return _Productions;}
   const Symbols &GetTerminalSymbols() const { return _TerminalSymbols; }
   const Symbols &GetNonTerminalSymbols() const { return _NonTerminalSymbols; }
private:
   std::string _Filename;
   Symbols _TerminalSymbols;
   Symbols _NonTerminalSymbols;
   Productions _Productions;
};
