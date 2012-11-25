#pragma once
#include <string>
#include <deque>
#include "Generator.h"
#include "Scanner.h"

class MicroCompiler
{
public:
   MicroCompiler(const std::string &grammer, const std::string &program);
   void Parse();
private:
   void _PrintBlock(const std::string &action, const std::string &remainingInput);
   void _UpdateSemanticStack(const size_t index);
   const std::string &_GetSemanticStackValue(const size_t index);
   void _HandleActionSymbol(const std::string &action);
   std::vector<size_t> _ParseArgumentIndices(const std::string &action);

   std::string _GrammerFile;
   std::string _ProgramFile;
   Scanner _Scanner;
   std::deque<std::string> _ParseStack;
   std::deque<std::string> _SemanticStack;
   size_t _LeftIndex, _RightIndex, _CurrentIndex, _TopIndex;
   Generator _CodeGenerator;
};
