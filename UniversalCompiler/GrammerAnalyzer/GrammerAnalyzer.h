#pragma once

#include <string>

class GrammerAnalyzer
{
public:
   GrammerAnalyzer(const std::string &filename);
private:
   std::string _Filename;
};
