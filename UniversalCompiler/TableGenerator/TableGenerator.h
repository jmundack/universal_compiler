#pragma once
#include <map>
#include <string>
#include "PredictGenerator.h"

typedef std::map<std::pair<std::string, std::string>, int> Table;
class TableGenerator
{
public:
   TableGenerator(const std::string &filename);
   void GenerateTable();
   const Table &GetTable() { GenerateTable();return _Table; }
private:
   std::string _Filename;
   PredictGenerator _PredictGenerator;
   Table _Table;
};
