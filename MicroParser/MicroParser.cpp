#include "MicroParser.h"
#include <iostream>

using namespace std;

MicroParser::MicroParser(const std::string &filename):_StateValid(false)
{
   _Tokens = Scanner(filename).GetTokens();
#ifdef DEBUG
  for (Tokens::const_iterator itr = _Tokens.begin(); itr != _Tokens.end(); ++itr)
    cout << *itr;
  cout << endl;
#endif
}

void MicroParser::Parse()
{
   _StateValid = true;
   _SystemGoal();
   if (!_Tokens.empty()) 
   {
      cerr << "Done paring but still have the following tokens left " << endl;
      for (Tokens::const_iterator itr = _Tokens.begin(); itr != _Tokens.end(); ++itr)
         cout << *itr << endl;
   }
}

void MicroParser::_Match(const Token legalToken)
{
   if (!_StateValid) return;
   if (_Tokens.empty()) { cerr << "Ran out of tokens" << endl; return; }
   if (_Tokens.front() != legalToken)
   {
      cerr << "Failed to match token expected : "<< legalToken << " but got " << _Tokens.front() << "instead " << endl;
      _StateValid = false;
   }
   cout << legalToken << endl;
   _Tokens.pop_front();
}

void MicroParser::_SystemGoal()
{
   if (!_StateValid) return;
   cout << " Parsing system_goal" << endl;
   _Program();
   _Match(EofSym);
}

void MicroParser::_Program()
{
   if (!_StateValid) return;
   cout << " Parsing program" << endl;
   _Match(BeginSym);
   _StatementList();
   _Match(EndSym);
}

void MicroParser::_StatementList()
{
   if (!_StateValid) return;
   cout << " Parsing statement_list" << endl;
   _Statement();
   if (_Tokens.empty()) { cerr << "Ran out of tokens" << endl; return; }
   switch(_Tokens.front())
   {
      case Id:
      case ReadSym:
      case WriteSym:
         _StatementList();
         break;
      default:
         break;
   }
}

void MicroParser::_Statement()
{
   if (!_StateValid) return;
   cout << " Parsing statement" << endl;
   if (_Tokens.empty()) { cerr << "Ran out of tokens" << endl; return; }
   switch(_Tokens.front())
   {
      case Id:
         _Ident();
         _Match(AssignOp);
         _Expression();
         _Match(SemiColon);
         break;
      case ReadSym:
         _Match(ReadSym);
         _Match(LParen);
         _IdList();
         _Match(RParen);
         _Match(SemiColon);
         break;
      case WriteSym:
         _Match(WriteSym);
         _Match(LParen);
         _ExpressionList();
         _Match(RParen);
         _Match(SemiColon);
         break;
      default:
         cerr << "Fail! " << endl;
         _StateValid = false;
         break;
   }
}

void MicroParser::_IdList()
{
   if (!_StateValid) return;
   cout << " Parsing id_list" << endl;
   _Ident();
   if (_Tokens.empty()) { cerr << "Ran out of tokens" << endl; return; }
   if (_Tokens.front() == Comma)
   {
      _Match(Comma);
      _IdList();
   }
}

void MicroParser::_ExpressionList()
{
   if (!_StateValid) return;
   cout << " Parsing expression_list" << endl;
   _Expression();
   if (_Tokens.empty()) { cerr << "Ran out of tokens" << endl; return; }
   if (_Tokens.front() == Comma)
   {
      _Match(Comma);
      _ExpressionList();
   }
}

void MicroParser::_Expression()
{
   if (!_StateValid) return;
   cout << " Parsing expression" << endl;
   _Primary();
   if (_Tokens.empty()) { cerr << "Ran out of tokens" << endl; return; }
   switch (_Tokens.front())
   {
      case PlusOp:
      case MinusOp:
         _AddOp();
         _Expression();
         break;
      default:
         break;
   }
}

void MicroParser::_Primary()
{
   if (!_StateValid) return;
   cout << " Parsing primary" << endl;
   if (_Tokens.empty()) { cerr << "Ran out of tokens" << endl; return; }
   switch(_Tokens.front())
   {
      case LParen:
         _Match(LParen);
         _Expression();
         _Match(RParen);
         break;
      case Id:
         _Ident();
         break;
      case IntLiteral:
         _Match(IntLiteral);
         break;
      default:
         cerr << "fail -- expected LParen | Id | IntLiteral but got " << _Tokens.front() << endl;
         _StateValid = false;
         break;
   }
}

void MicroParser::_Ident()
{
   if (!_StateValid) return;
   cout << " Parsing ident" << endl;
   _Match(Id);
}

void MicroParser::_AddOp()
{
   cout << " Parsing add_op" << endl;
   if (_Tokens.empty()) { cerr << "Ran out of tokens" << endl; return; }
   switch (_Tokens.front())
   {
      case PlusOp:
      case MinusOp:
         _Match(_Tokens.front());
         break;
      default:
         cerr << "fail -- expected PlusOp | MinusOp but got " << _Tokens.front() << endl;
         _StateValid = false;
         break;
   }
}
