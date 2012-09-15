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

void MicroParser::_PrintValidation()
{
   cout << " ************ OUTPUT *********** " << endl;
   if (!_StateValid) 
   {
      cout << "Code had syntax errors! " << endl;
      return;
   }
   string s = "<system_goal>";
   while (true)
   {
      cout << s << endl;
      size_t beg = s.find_first_of('<');
      size_t end = s.find_first_of('>');
      if (beg == string::npos || end == string::npos) break;
      end -= beg; // calculate thr num characters
      end ++; // include the '>'
      string key = s.substr(beg, end);
      if (_Tracker[key].empty())  cerr << "Key : " << key << " is empty" << endl;
      string value = _Tracker[key].front();
      _Tracker[key].pop_front();
      s.replace(s.find_first_of('<'), key.size(), value);
   }
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
   _PrintValidation();
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
   _Tracker["<system_goal>"].push_back("<program> $");
   if (!_StateValid) return;
   cout << " Parsing system_goal" << endl;
   _Program();
   _Match(EofSym);
}

void MicroParser::_Program()
{
   if (!_StateValid) return;
   cout << " Parsing program" << endl;
   _Tracker["<program>"].push_back("begin <statement_list> end");
   _Match(BeginSym);
   _StatementList();
   _Match(EndSym);
}

void MicroParser::_StatementList()
{
   if (!_StateValid) return;
   cout << " Parsing statement_list" << endl;
   const size_t pos = _Tracker["<statement_list>"].size();
   _Statement();
   string s = "<statement>";
   if (_Tokens.empty()) { cerr << "Ran out of tokens" << endl; return; }
   switch(_Tokens.front())
   {
      case Id:
      case ReadSym:
      case WriteSym:
         _StatementList();
         s += "<statement_list>";
         break;
      default:
         break;
   }
   _Tracker["<statement_list>"].insert(_Tracker["<statement_list>"].begin() + pos, s);
}

void MicroParser::_Statement()
{
   if (!_StateValid) return;
   cout << " Parsing statement" << endl;
   if (_Tokens.empty()) { cerr << "Ran out of tokens" << endl; return; }
   const size_t pos = _Tracker["<statement>"].size();
   string s;
   switch(_Tokens.front())
   {
      case Id:
         _Ident();
         _Match(AssignOp);
         _Expression();
         _Match(SemiColon);
         s += "<ident> := <expression>;";
         break;
      case ReadSym:
         _Match(ReadSym);
         _Match(LParen);
         _IdList();
         _Match(RParen);
         _Match(SemiColon);
         s += "read (<id_list>);";
         break;
      case WriteSym:
         _Match(WriteSym);
         _Match(LParen);
         _ExpressionList();
         _Match(RParen);
         _Match(SemiColon);
         s += "write(<expression_list>);";
         break;
      default:
         cerr << "Fail! " << endl;
         _StateValid = false;
         break;
   }
   _Tracker["<statement>"].insert(_Tracker["<statement>"].begin() + pos, s);
}

void MicroParser::_IdList()
{
   if (!_StateValid) return;
   if (_Tokens.empty()) { cerr << "Ran out of tokens" << endl; return; }
   cout << " Parsing id_list" << endl;
   const size_t pos = _Tracker["<id_list>"].size();
   _Ident();
   string s = "<ident>";
   if (_Tokens.front() == Comma)
   {
      _Match(Comma);
      _IdList();
      s += ", <id_list>";
   }
   _Tracker["<id_list>"].insert(_Tracker["<id_list>"].begin() + pos, s);
}

void MicroParser::_ExpressionList()
{
   if (!_StateValid) return;
   cout << " Parsing expression_list" << endl;
   if (_Tokens.empty()) { cerr << "Ran out of tokens" << endl; return; }
   const size_t pos = _Tracker["<expression_list>"].size();
   _Expression();
   string s = "<expression>";
   if (_Tokens.front() == Comma)
   {
      _Match(Comma);
      _ExpressionList();
      s += ", <expression_list>";
   }
   _Tracker["<expression_list>"].insert(_Tracker["<expression_list>"].begin() + pos, s);
}

void MicroParser::_Expression()
{
   if (!_StateValid) return;
   if (_Tokens.empty()) { cerr << "Ran out of tokens" << endl; return; }
   cout << " Parsing expression -- next token : " << _Tokens.front() << endl;
   const size_t pos = _Tracker["<expression>"].size();
   
   _Primary();
   string s = "<primary>";
   switch (_Tokens.front())
   {
      case PlusOp:
      case MinusOp:
         _AddOp();
         _Expression();
         s += "<add_op> <expression>";
         break;
      default:
         break;
   }
   _Tracker["<expression>"].insert(_Tracker["<expression>"].begin() + pos, s);
}

void MicroParser::_Primary()
{
   if (!_StateValid) return;
   cout << " Parsing primary" << endl;
   if (_Tokens.empty()) { cerr << "Ran out of tokens" << endl; return; }
   const size_t pos = _Tracker["<primary>"].size();
   string s;
   switch(_Tokens.front())
   {
      case LParen:
         _Match(LParen);
         _Expression();
         _Match(RParen);
         s = "(<expression>)";
         break;
      case Id:
         _Ident();
         s = "<ident>";
         break;
      case IntLiteral:
         _Match(IntLiteral);
         s = "IntLiteral";
         break;
      default:
         cerr << "fail -- expected LParen | Id | IntLiteral but got " << _Tokens.front() << endl;
         _StateValid = false;
         break;
   }
   _Tracker["<primary>"].insert(_Tracker["<primary>"].begin() + pos, s);
}

void MicroParser::_Ident()
{
   if (!_StateValid) return;
   cout << " Parsing ident" << endl;
   _Tracker["<ident>"].push_back("Id");
   _Match(Id);
}

void MicroParser::_AddOp()
{
   cout << " Parsing add_op" << endl;
   if (_Tokens.empty()) { cerr << "Ran out of tokens" << endl; return; }
   switch (_Tokens.front())
   {
      case PlusOp:
         _Tracker["<add_op>"].push_back("+");
         _Match(_Tokens.front());
         break;
      case MinusOp:
         _Tracker["<add_op>"].push_back("-");
         _Match(_Tokens.front());
         break;
      default:
         cerr << "fail -- expected PlusOp | MinusOp but got " << _Tokens.front() << endl;
         _StateValid = false;
         break;
   }
}
