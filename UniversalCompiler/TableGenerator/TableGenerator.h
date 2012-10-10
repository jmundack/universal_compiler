#pragma once
#include <map>
#include <string>
#include "PredictGenerator.h"

class TableGenerator
{
public:
   TableGenerator(const std::string &filename);
   void GenerateTable();
private:
   std::string _Filename;
   PredictGenerator _PredictGenerator;
   typedef std::map<std::pair<std::string, std::string>, int> Table;
   Table _Table;
};
