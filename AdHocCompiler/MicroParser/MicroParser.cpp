#include "MicroParser.h"
#include <iostream>
#include <algorithm>
#include <iterator>
#include <stdlib.h>

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

MicroParser::MicroParser(const std::string &filename):_Filename(filename),_Scanner(filename),_Generator(_GenerateOutputFilename(filename))
{
}

void MicroParser::_PrintValidation()
{
   cout << " ************ OUTPUT *********** " << endl;
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

void MicroParser::_PrintGeneratedCode()
{
   cout << " ************ GENERTED CODE *********** " << endl;
   string cmd = "cat " + _GenerateOutputFilename(_Filename);
   cout << "Running : " << cmd << endl;
   int result = system(cmd.c_str());
   cout << "System call to cat : " << result << endl;
}

void MicroParser::Parse()
{
   _Scanner.ReadNextToken();
   _SystemGoal();
   cout << "Getting remaining tokens... " << endl;
   Tokens tokens = _Scanner.GetTokens();
   if (!tokens.empty()) 
   {
      cerr << "Done paring but still have the following tokens left " << endl;
      for (Tokens::const_iterator itr = tokens.begin(); itr != tokens.end(); ++itr)
         cout << *itr << endl;
   }
   _PrintValidation();
   _PrintGeneratedCode();
   cout << "Error detected : " << endl << _Errors.str() << endl;
}

void MicroParser::_Match(const Token legalToken)
{
   cout << "Match : " << legalToken << " nextToken " << _Scanner.GetNextToken() << endl;
   if (_Scanner.GetNextToken() != legalToken)
   {
      cout << "Failed Match : " << legalToken << " nextToken " << _Scanner.GetNextToken() << endl;
      if (legalToken == EofSym || legalToken == SemiColon)
      {
         while (legalToken != _Scanner.GetNextToken())
         {
            if (legalToken == EofSym || legalToken == SemiColon) break;
            cout << "Skipping token : " << _Scanner.GetNextToken() << endl;
            _Errors << "Skipping token : " << _Scanner.GetNextToken() << endl;
            _Scanner.ReadNextToken();
         }
      }
      else
         _Scanner.SetNextToken(legalToken);
      /*
      else
      {
         cerr << "Failed to match token expected : "<< legalToken << " but got " << _Scanner.GetNextToken() << "instead " << endl;
      }*/
   }
   else
      _Scanner.ReadNextToken();
   cout << "DONE Match : " << legalToken << endl;
}

void MicroParser::_SystemGoal()
{
   _Tracker["<system_goal>"].push_back("<program> $");
   cout << " Parsing system_goal" << endl;
   _Program();
   _Match(EofSym);
   _Generator.Finish();
}

void MicroParser::_Program()
{
   cout << " Parsing program" << endl;
   _Tracker["<program>"].push_back("begin <statement_list> end");
   _Generator.Start();
   _CheckInput(__FUNCTION__,Tokens(1,BeginSym), Tokens(1,EofSym), Tokens(1,EofSym));
   _Match(BeginSym);
   _StatementList();
   _Match(EndSym);
}

void MicroParser::_StatementList()
{
   cout << " Parsing statement_list" << endl;
   const size_t pos = _Tracker["<statement_list>"].size();
   Tokens validSet;
   validSet.push_back(Id);
   validSet.push_back(ReadSym);
   validSet.push_back(WriteSym);
   _CheckInput(__FUNCTION__,validSet, Tokens(1,EndSym), Tokens(1,EofSym));
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
   cout << " Parsing statement" << endl;
   const size_t pos = _Tracker["<statement>"].size();

   Tokens validSet;
   validSet.push_back(Id);
   validSet.push_back(ReadSym);
   validSet.push_back(WriteSym);
   _CheckInput(__FUNCTION__,validSet, Tokens(1,EndSym), Tokens(1,EofSym));
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
         break;
   }
   _Tracker["<statement>"].insert(_Tracker["<statement>"].begin() + pos, s);
}

void MicroParser::_IdList()
{
   cout << " Parsing id_list" << endl;
   const size_t pos = _Tracker["<id_list>"].size();
   _CheckInput(__FUNCTION__,Tokens(1,Id), Tokens(1,RParen), Tokens(1,EofSym));
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
   cout << " Parsing expression_list" << endl;
   const size_t pos = _Tracker["<expression_list>"].size();
   Tokens validSet;
   validSet.push_back(Id);
   validSet.push_back(IntLiteral);
   validSet.push_back(LParen);
   _CheckInput(__FUNCTION__,validSet, Tokens(1,RParen), Tokens(1,EofSym));
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
   cout << " Parsing expression -- next token : " << _Scanner.GetNextToken() << endl;
   const size_t pos = _Tracker["<expression>"].size();

   Tokens validSet,followSet;
   validSet.push_back(Id);
   validSet.push_back(IntLiteral);
   validSet.push_back(LParen);
   followSet.push_back(Comma);
   followSet.push_back(SemiColon);
   followSet.push_back(RParen);
   _CheckInput(__FUNCTION__,validSet, followSet, Tokens(1,EofSym));
  
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
   cout << " Parsing primary" << endl;
   const size_t pos = _Tracker["<primary>"].size();
   Tokens validSet,followSet;
   validSet.push_back(Id);
   validSet.push_back(IntLiteral);
   validSet.push_back(LParen);
   followSet.push_back(Comma);
   followSet.push_back(SemiColon);
   followSet.push_back(RParen);
   followSet.push_back(PlusOp);
   followSet.push_back(MinusOp);
   _CheckInput(__FUNCTION__,validSet, followSet, Tokens(1,EofSym));
 
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
         break;
   }
   _Tracker["<primary>"].insert(_Tracker["<primary>"].begin() + pos, s);
}

void MicroParser::_Ident(ExpressionRec &e)
{
   cout << " Parsing ident" << endl;
   _Tracker["<ident>"].push_back("Id");
   Tokens followSet;
   followSet.push_back(Comma);
   followSet.push_back(SemiColon);
   followSet.push_back(RParen);
   _CheckInput(__FUNCTION__,Tokens(1,Id), followSet, Tokens(1,EofSym));
   _Generator.ProcessID(_Scanner.GetBuffer(), e);
   _Match(Id);
}

void MicroParser::_AddOp(OperationRec &op)
{
   cout << " Parsing add_op" << endl;
   Tokens validSet,followSet, headerSet;
   validSet.push_back(PlusOp);
   validSet.push_back(MinusOp);
   followSet.push_back(LParen);
   followSet.push_back(Id);
   followSet.push_back(IntLiteral);
   headerSet.push_back(EndSym);
   headerSet.push_back(EofSym);
   headerSet.push_back(SemiColon);
   _CheckInput(__FUNCTION__, validSet, followSet, headerSet);
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
         break;
   }
}


void MicroParser::_CheckInput(const string &name,
                              const Tokens &validSet, 
                              const Tokens &followSet,
                              const Tokens &headerSet)
{
   Token nextToken(_Scanner.GetNextToken());
   cout << "_CheckInput : " << name << " nextToken : " << nextToken << endl;
      Tokens t(validSet);
      for (size_t i = 0; i < followSet.size(); i++)
         t.push_back(followSet.at(i));
      for (size_t i = 0; i < headerSet.size(); i++)
         t.push_back(headerSet.at(i));
      cout << "************** Valid set : ";
      copy(validSet.begin(), validSet.end(), std::ostream_iterator<Token>(cout," "));
      cout << endl;
      cout << "************* follow set : ";
      copy(followSet.begin(), followSet.end(), std::ostream_iterator<Token>(cout," "));
      cout << endl;
      cout << "************** header set : ";
      copy(headerSet.begin(), headerSet.end(), std::ostream_iterator<Token>(cout," "));
      cout << endl;
      cout << "************** merged set : ";
      copy(t.begin(), t.end(), std::ostream_iterator<Token>(cout," "));
      cout << endl;
 
   if (find(validSet.begin(), validSet.end(), nextToken) == validSet.end())
   {
      cout << "Invalid Token : " << nextToken << endl;
      _Errors << "Skipping token : " << nextToken << endl;
      while(find(t.begin(), t.end(), nextToken) == t.end())
      {
         cout << "Skipping token : " << nextToken << endl;
         _Scanner.ReadNextToken();
         nextToken = _Scanner.GetNextToken();
         _Errors << "Skipping token : " << nextToken << endl;
      }
   }
   cout << "DONE _CheckInput" << endl;
}
