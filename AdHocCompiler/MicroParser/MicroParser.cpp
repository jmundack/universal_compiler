#include "MicroParser.h"
#include <iostream>

using namespace std;

string _GenerateOutputFilename(const string &inputFilename)
{
   string outputFilename = inputFilename;
   const size_t pos = inputFilename.find_last_of('/');
   if (pos != string::npos)
   {
      outputFilename = inputFilename.substr(pos+1);
   }
   outputFilename += ".out";
   return outputFilename;
}

MicroParser::MicroParser(const std::string &filename):_StateValid(false), _Scanner(filename),_Generator(_GenerateOutputFilename(filename))
{
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
   _Scanner.ReadNextToken();
   _SystemGoal();
   Tokens tokens = _Scanner.GetTokens();
   if (!tokens.empty()) 
   {
      cerr << "Done paring but still have the following tokens left " << endl;
      for (Tokens::const_iterator itr = tokens.begin(); itr != tokens.end(); ++itr)
         cout << *itr << endl;
   }
   _PrintValidation();
}

void MicroParser::_Match(const Token legalToken)
{
   if (!_StateValid) return;
   if (_Scanner.GetNextToken() != legalToken)
   {
      cerr << "Failed to match token expected : "<< legalToken << " but got " << _Scanner.GetNextToken() << "instead " << endl;
      _StateValid = false;
   }
   cout << "Legal Token : " << legalToken << endl;
   _Scanner.ReadNextToken();
}

void MicroParser::_SystemGoal()
{
   _Tracker["<system_goal>"].push_back("<program> $");
   if (!_StateValid) return;
   cout << " Parsing system_goal" << endl;
   _Program();
   _Match(EofSym);
   _Generator.Finish();
}

void MicroParser::_Program()
{
   if (!_StateValid) return;
   cout << " Parsing program" << endl;
   _Tracker["<program>"].push_back("begin <statement_list> end");
   _Generator.Start();
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
   switch(_Scanner.GetNextToken())
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
   const size_t pos = _Tracker["<statement>"].size();
   string s;
   ExpressionRec source, target;
   switch(_Scanner.GetNextToken())
   {
      case Id:
         _Ident(target);
         _Match(AssignOp);
         _Expression(source);
         _Generator.Assign(source, target);
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
   cout << " Parsing id_list" << endl;
   const size_t pos = _Tracker["<id_list>"].size();
   ExpressionRec identifier;
   _Ident(identifier);
   _Generator.ReadID(identifier);
   string s = "<ident>";
   if (_Scanner.GetNextToken() == Comma)
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
   const size_t pos = _Tracker["<expression_list>"].size();
   ExpressionRec e;
   _Expression(e);
   _Generator.WriteID(e);
   string s = "<expression>";
   if (_Scanner.GetNextToken() == Comma)
   {
      _Match(Comma);
      _ExpressionList();
      s += ", <expression_list>";
   }
   _Tracker["<expression_list>"].insert(_Tracker["<expression_list>"].begin() + pos, s);
}

void MicroParser::_Expression(ExpressionRec &e)
{
   if (!_StateValid) return;
   cout << " Parsing expression -- next token : " << _Scanner.GetNextToken() << endl;
   const size_t pos = _Tracker["<expression>"].size();
  
   ExpressionRec e1, e2;
   OperationRec op;
   _Primary(e1);
   string s = "<primary>";
   switch (_Scanner.GetNextToken())
   {
      case PlusOp:
      case MinusOp:
         _AddOp(op);
         _Expression(e2);
         e = _Generator.GenerateInFix(e1,op,e2);
         s += "<add_op> <expression>";
         break;
      default:
         e = e1;
         break;
   }
   _Tracker["<expression>"].insert(_Tracker["<expression>"].begin() + pos, s);
}

void MicroParser::_Primary(ExpressionRec &e)
{
   if (!_StateValid) return;
   cout << " Parsing primary" << endl;
   const size_t pos = _Tracker["<primary>"].size();
   string s;
   switch(_Scanner.GetNextToken())
   {
      case LParen:
         _Match(LParen);
         _Expression(e);
         _Match(RParen);
         s = "(<expression>)";
         break;
      case Id:
         _Ident(e);
         s = "<ident>";
         break;
      case IntLiteral:
         _Generator.ProcessLiteral(_Scanner.GetBuffer(), e);
         _Match(IntLiteral);
         s = "IntLiteral";
         break;
      default:
         cerr << "fail -- expected LParen | Id | IntLiteral but got " << _Scanner.GetNextToken() << endl;
         _StateValid = false;
         break;
   }
   _Tracker["<primary>"].insert(_Tracker["<primary>"].begin() + pos, s);
}

void MicroParser::_Ident(ExpressionRec &e)
{
   if (!_StateValid) return;
   cout << " Parsing ident" << endl;
   _Tracker["<ident>"].push_back("Id");
   _Generator.ProcessID(_Scanner.GetBuffer(), e);
   _Match(Id);
}

void MicroParser::_AddOp(OperationRec &op)
{
   cout << " Parsing add_op" << endl;
   switch (_Scanner.GetNextToken())
   {
      case PlusOp:
         _Tracker["<add_op>"].push_back("+");
         _Match(_Scanner.GetNextToken());
         _Generator.ProcessOperationRec(PlusOp, op);
         break;
      case MinusOp:
         _Tracker["<add_op>"].push_back("-");
         _Match(_Scanner.GetNextToken());
         _Generator.ProcessOperationRec(MinusOp, op);
         break;
      default:
         cerr << "fail -- expected PlusOp | MinusOp but got " << _Scanner.GetNextToken() << endl;
         _StateValid = false;
         break;
   }
}
