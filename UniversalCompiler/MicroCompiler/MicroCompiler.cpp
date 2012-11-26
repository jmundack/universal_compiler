#include "MicroCompiler.h"
#include "TableGenerator.h"
#include "PredictGenerator.h"
#include <stack>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <boost/lexical_cast.hpp>

using namespace std;

namespace
{
   const bool Debug(false);
   string _GetTokenStr(const Token token)
   {
      string s = "ERROR";
      switch(token)
      {
         case BeginSym : s = "begin"; break;
         case EndSym : s = "end"; break;
         case ReadSym : s = "read"; break;
         case WriteSym : s = "write"; break;
         case Id : s = "Id"; break;
         case IntLiteral : s = "IntLiteral"; break;
         case LParen : s = "("; break;
         case RParen : s = ")"; break;
         case SemiColon : s = ";"; break;
         case Comma : s = ","; break;
         case AssignOp : s = ":="; break;
         case PlusOp : s = "PlusOp"; break;
         case MinusOp : s = "MinusOp"; break;
         case EofSym : s = "$"; break;
         default: cerr << "Invalid token : " << token << endl; break;
      }
      return s;
   }
}

string _Buffer;
map<size_t,ExpressionRec> _Records;
vector<OperationRec> _OpRec;
const char *OutputFilename("generated_code.out");

void MicroCompiler::_PrintBlock(const string &action, const string &remainingInput)
{
   cout << "******************************************" << endl;
   cout << action << endl;
   cout << "---> Input : " << remainingInput << endl;
   cout << "---> PS : ";
   std::copy(_ParseStack.begin(), _ParseStack.end(), std::ostream_iterator<string>(std::cout, " "));
   cout << endl;
   cout << "---> SS : ";
   std::copy(_SemanticStack.begin(), _SemanticStack.end(), std::ostream_iterator<string>(std::cout, " "));
   cout << endl;
   cout << "---> Indices : Left(" << _LeftIndex << ") right (" << _RightIndex << ") current(" << _CurrentIndex << ") top(" << _TopIndex << ")" << endl; 
   cout << "---> Code Generates : " << endl;
   cout << _CodeGenerator.GetCodeGenerated() << endl;
   cout << "******************************************" << endl;
}

MicroCompiler::MicroCompiler(const string &grammer, const string &program):_GrammerFile(grammer),_ProgramFile(program),_Scanner(program),_CodeGenerator(OutputFilename)
{}

void MicroCompiler::Parse()
{
   Productions productions(GrammerAnalyzer(_GrammerFile).GetProductions());
   Table table(TableGenerator(_GrammerFile).GetTable());
   string remainingInput(_Scanner.GetRemainingInput());
   _Scanner.ReadNextToken();
   _Buffer = _Scanner.GetBuffer();
   PredictGenerator p(_GrammerFile);
   p.Print();

   _ParseStack.push_front("<system_goal>");
   _SemanticStack.push_front("<system_goal>");
   _LeftIndex = 0;
   _RightIndex = 0;
   _CurrentIndex = 1;
   _TopIndex = 2;

   stack<string> s;
   s.push("<system_goal>");
   cout << "***********************************************" << endl;
   _PrintBlock("Enrty", remainingInput);
   while(!_ParseStack.empty())
   {
      string action("INVALID");
      string topSymbol = _ParseStack.at(0);
      if (topSymbol.at(0) == '<')
      {
         const pair<string,string> key(make_pair(topSymbol, _GetTokenStr(_Scanner.GetNextToken())));
         const size_t productionNum(table[key]);
         if (productionNum == 0)
         {
            cerr << "Invalid data -- non terminal : " << topSymbol << " token : " << _GetTokenStr(_Scanner.GetNextToken()) << endl;
            return;
         }
         else
         {
            action = "NonTerm : T(" + topSymbol + "," + key.second + ") = " + boost::lexical_cast<string>(productionNum);
            _ParseStack.pop_front();
            _ParseStack.push_front(boost::lexical_cast<string>(_TopIndex));
            _ParseStack.push_front(boost::lexical_cast<string>(_CurrentIndex));
            _ParseStack.push_front(boost::lexical_cast<string>(_RightIndex));
            _ParseStack.push_front(boost::lexical_cast<string>(_LeftIndex));
            _ParseStack.push_front("EOP");
            if(Debug) cout << "************** Getting production : " << productionNum << " of " << (productions.size() + 1) << endl;
            const pair<string, vector<string> > &production(productions.at(productionNum -1));
            if(Debug) cout << "************* Getting rhs of production" << endl;
            const vector<string> &rhs(production.second);
            for(vector<string>::const_reverse_iterator itr = rhs.rbegin(); itr != rhs.rend(); ++itr)
               _ParseStack.push_front(*itr);
            size_t numNonActionSymbols(0);
            for(vector<string>::const_iterator itr = rhs.begin(); itr != rhs.end(); ++itr)
               if (itr->at(0) != '#') // skip action symbols
               {
                  numNonActionSymbols++;
                  _SemanticStack.push_front(*itr);
               }
            _LeftIndex = _CurrentIndex;
            _RightIndex = _TopIndex;
            _CurrentIndex = _RightIndex;
            _TopIndex += numNonActionSymbols;
         }
      }
      else
      {
         const Symbols &terminalSymbols(p.GetTerminalSymbols());
         if (terminalSymbols.find(topSymbol) != terminalSymbols.end() || topSymbol == "lamda")
         {
            /*
            if (topSymbol != _Scanner.GetNextToken())
            {
               cerr << "Synatx Error -- expected : " << topSymbol << " but got " << _Scanner.GetNextToken() << endl;
               return;
            }*/
            action = "Term : " + _ParseStack.front();
            if (topSymbol != "lamda")
            {
               _UpdateSemanticStack(_CurrentIndex);
               remainingInput = _Scanner.GetRemainingInput();
               _Buffer = _Scanner.GetBuffer();
               _Scanner.ReadNextToken();
            }
            if(Debug) cout << "Poping for match" << endl;
            _CurrentIndex++;
            _ParseStack.pop_front();
         }
         else 
         {
            if (topSymbol == "EOP")
            {
               action = "EOP : (" + _ParseStack.at(1) + ',' + _ParseStack.at(2) +',' + _ParseStack.at(3) +','+ _ParseStack.at(4) + ")";
               _ParseStack.pop_front(); // pop EOP
               // restore all indices
               _LeftIndex = boost::lexical_cast<size_t>(_ParseStack.front());
               _ParseStack.pop_front();
               _RightIndex = boost::lexical_cast<size_t>(_ParseStack.front());
               _ParseStack.pop_front();
               _CurrentIndex = boost::lexical_cast<size_t>(_ParseStack.front());
               _ParseStack.pop_front();
               _TopIndex = boost::lexical_cast<size_t>(_ParseStack.front());
               _ParseStack.pop_front();
               // Done restoring indices
               _CurrentIndex ++;
               while(_SemanticStack.size() != _TopIndex -1)
               {
                  cout << "Popping : " << _SemanticStack.front()  << " from _Semantic stack " << endl;
                  _SemanticStack.pop_front();
               }
            }
            else
            {
               action = "Action : " + _ParseStack.front();
               _HandleActionSymbol(_ParseStack.front());
               _ParseStack.pop_front();
            }
         }
      }
      _PrintBlock(action, remainingInput);
   }
}

void MicroCompiler::_UpdateSemanticStack(const size_t index)
{
   deque<string>::iterator itr = _SemanticStack.end() - index;
//   *itr = _Scanner.GetBuffer();
   *itr = _Buffer;
}

void MicroCompiler::_HandleActionSymbol(const std::string &action)
{
   const vector<size_t> &arguments(_ParseArgumentIndices(action));
   string function = action.substr(0,action.find_first_of('('));
   cout << "Function := " << function << endl;
   cout << "(";
   std::copy(arguments.begin(), arguments.end(), std::ostream_iterator<size_t>(std::cout, ","));
   cout << ")" << endl;
   if (function == "#Start")
   {
      _CodeGenerator.Start();
   }
   else if (function == "#Finish")
   {
      _CodeGenerator.Finish();
   }
   else if (function == "#Assign")
   {
      cout << " Records : " << endl;
      for (map<size_t, ExpressionRec>::const_iterator itr = _Records.begin(); itr != _Records.end(); ++itr)
      {
         cout << "index : " << itr->first << " name : " << itr->second.GetStr() << endl;
      }
      _CodeGenerator.Assign(_Records.at(arguments.at(1)), _Records.at(arguments.at(0)));

   }
   else if (function == "#ReadId")
   {
      _CodeGenerator.ReadID(_Records.at(arguments.at(0)));
   }
   else if (function == "#WriteExpr")
   {
      _CodeGenerator.WriteID(_Records.at(arguments.at(0)));
   }
   else if (function == "#GenInfix")
   {
      cout << " Records : " << endl;
      for (map<size_t, ExpressionRec>::const_iterator itr = _Records.begin(); itr != _Records.end(); ++itr)
      {
         cout << "index : " << itr->first << " name : " << itr->second.GetStr() << endl;
      }
      _Records[arguments.at(0)] = _CodeGenerator.GenerateInFix(_Records.at(arguments.at(0)), _OpRec.back(), _Records.at(arguments.at(2)));
      _OpRec.pop_back();

   }
   else if (function == "#ProcessLiteral")
   {
      _UpdateSemanticStack(arguments.at(0));
      ExpressionRec e;
      _CodeGenerator.ProcessLiteral(_GetSemanticStackValue(arguments.at(0)), e);
      cout << "Adding record for : " << arguments.at(0) << endl;
      _Records[arguments.at(0)] = e;
   }
   else if (function == "#ProcessOp")
   {
      OperationRec opRec;
      _CodeGenerator.ProcessOperationRec(_Buffer == "+" ? PlusOp : MinusOp, opRec);
      _OpRec.push_back(opRec);
   }
   else if (function == "#ProcessId")
   {
      _UpdateSemanticStack(arguments.at(0));
      ExpressionRec e;
      _CodeGenerator.ProcessID(_GetSemanticStackValue(arguments.at(0)), e);
      cout << "Adding record for : " << arguments.at(0) << endl;
      _Records[arguments.at(0)] = e;
   }
   else if (function == "#Copy")
   {
      cout << "Copying record from : " << arguments.at(0) << " to " << arguments.at(1) << endl;
      _Records[arguments.at(1)] = _Records[arguments.at(0)];
   }
   else
   {
      throw("Unknown Action!");
   }
}

vector<size_t> MicroCompiler::_ParseArgumentIndices(const std::string &action)
{
   vector<size_t> indices;
   bool isIndex(false);
   for(size_t i = 0; i < action.size(); i++)
   {
      if (action.at(i) == '$') 
      {
         if (isIndex)
         {
            indices.push_back(_LeftIndex);
            isIndex = false;
         }
         else
            isIndex = true;
            continue;
      }
      else if(isIndex)
      {
         const size_t comma = action.find_first_of(',', i);
         const size_t rParen = action.find_first_of(')', i);
         const size_t numBytes = (comma < rParen ? comma : rParen) - i;
         const string num(action.substr(i, numBytes));
         indices.push_back(_RightIndex + boost::lexical_cast<size_t>(num) -1);
         isIndex = false;
      }
   }
   if (Debug)
   {
      cout << "Indices for : " << action << " is ";
      std::copy(indices.begin(), indices.end(), std::ostream_iterator<size_t>(std::cout, ","));
      cout << endl;
   }
   return indices;

}

const string &MicroCompiler::_GetSemanticStackValue(const size_t index)
{
   deque<string>::iterator itr = _SemanticStack.end() - index;
   return *itr;
}
