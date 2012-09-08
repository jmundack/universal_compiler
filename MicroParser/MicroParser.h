#pragma once

#include "Scanner.h"
#include <map>
#include <functional>

class MicroParser
{
public:
   MicroParser(const std::string &filename);
   void Parse();
private:
   void _Match(const Token legalToken);
   void _SystemGoal();
   void _Program();
   void _StatementList();
   void _Statement();
   void _IdList();
   void _ExpressionList();
   void _Expression();
   void _Primary();
   void _Ident();
   void _AddOp();

   void _PrintState() const;
   Token _CurrentToken;
   Tokens _Tokens;
};
