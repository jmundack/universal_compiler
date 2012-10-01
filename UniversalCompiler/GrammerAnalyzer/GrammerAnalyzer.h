#pragma once

#include <string>

class GrammerAnalyzer
{
public:
   GrammerAnalyzer(const std::string &filename);
   void Analyze();
private:
   std::string _Filename;
};
