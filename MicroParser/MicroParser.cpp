#include "MicroParser.h"
#include <iostream>

using namespace std;

MicroParser::MicroParser(const std::string &filename)
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
   _SystemGoal();
}

void MicroParser::_Match(const Token legalToken)
{
   _CurrentToken = _Tokens.front();
   if (_CurrentToken != legalToken)
      cerr << "Failed to match token : " << _CurrentToken << endl;
   cout << legalToken;
   _Tokens.pop_front();
}

void MicroParser::_SystemGoal()
{
   _Program();
   _Match(EofSym);
}

void MicroParser::_Program()
{
   _Match(BeginSym);
   _StatementList();
   _Match(EndSym);
}

void MicroParser::_StatementList()
{
   _Statement();
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
         break;
   }
}

void MicroParser::_IdList()
{
   _Ident();
   if (_Tokens.front() == Comma)
   {
      _Match(Comma);
      _IdList();
   }
}

void MicroParser::_ExpressionList()
{
   _Expression();
   if (_Tokens.front() == Comma)
   {
      _Match(Comma);
      _ExpressionList();
   }
}

void MicroParser::_Expression()
{
   _Primary();
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
         cerr << "fail" << endl;
   }
}

void MicroParser::_Ident()
{
   _Match(Id);
}

void MicroParser::_AddOp()
{
   switch (_Tokens.front())
   {
      case PlusOp:
      case MinusOp:
         _Match(_Tokens.front());
         break;
      default:
         cerr << "fail" << endl;
   }
}
