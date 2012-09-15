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

   void _PrintValidation();
   Tokens _Tokens;
   bool _StateValid;

   typedef std::map<std::string, std::deque<std::string> > Tracker;
   Tracker _Tracker;

};
