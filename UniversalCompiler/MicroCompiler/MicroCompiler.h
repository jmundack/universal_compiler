#pragma once
#include <string>
#include <deque>

class MicroCompiler
{
public:
   MicroCompiler(const std::string &grammer, const std::string &program);
   void Parse();
private:
   void _PrintBlock(const std::string &action, const std::string &remainingInput);
   std::string _GrammerFile;
   std::string _ProgramFile;
   std::deque<std::string> _ParseStack;
   std::deque<std::string> _SemanticStack;
   size_t _LeftIndex, _RightIndex, _CurrentIndex, _TopIndex;
};
