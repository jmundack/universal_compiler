#pragma once
#include <string>

class ParserDriver
{
public:
   ParserDriver(const std::string &grammer, const std::string &program);
   void Parse();
private:
   std::string _GrammerFile;
   std::string _ProgramFile;
};
