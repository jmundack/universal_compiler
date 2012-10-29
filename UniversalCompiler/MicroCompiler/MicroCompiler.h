#pragma once
#include <string>

class MicroCompiler
{
public:
   MicroCompiler(const std::string &grammer, const std::string &program);
   void Parse();
private:
   std::string _GrammerFile;
   std::string _ProgramFile;
};
