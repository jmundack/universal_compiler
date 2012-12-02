#pragma once

#include "Scanner.h"
#include "Generator.h"
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
   void _Expression(ExpressionRec &e);
   void _Primary(ExpressionRec &e);
   void _Ident(ExpressionRec &e);
   void _AddOp(OperationRec &op);

   void _PrintValidation();
   void _PrintGeneratedCode();

   void _CheckInput(const Tokens &validSet, 
                    const Tokens &followSet,
                    const Tokens &heaserSet);
                  

   typedef std::map<std::string, std::deque<std::string> > Tracker;
   std::string _Filename;
   bool _StateValid;
   Tracker _Tracker;
   Scanner _Scanner;
   Generator _Generator;
};
