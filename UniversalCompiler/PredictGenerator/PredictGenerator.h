#pragma once

#include "GrammerAnalyzer.h"
#include <map>

class PredictGenerator
{
public:
   PredictGenerator(const std::string &filename);
   void PrintPredictSet();
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
};
