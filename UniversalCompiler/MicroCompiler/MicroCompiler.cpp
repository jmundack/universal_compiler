#include "MicroCompiler.h"
#include "TableGenerator.h"
#include "Scanner.h"
#include "PredictGenerator.h"
#include <stack>
#include <sstream>
#include <algorithm>
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

void MicroCompiler::_PrintBlock(const string &action, const string &remainingInput)
{
   cout << "******************************************" << endl;
   cout << action << endl;
   cout << "Input : " << remainingInput << endl;
   cout << "PS : ";
   std::copy(_ParseStack.begin(), _ParseStack.end(), std::ostream_iterator<string>(std::cout, ""));
   cout << endl;
   cout << "SS : ";
   std::copy(_SemanticStack.begin(), _SemanticStack.end(), std::ostream_iterator<string>(std::cout, ""));
   cout << endl;
   cout << "Indices : Left(" << _LeftIndex << ") right (" << _RightIndex << ") current(" << _CurrentIndex << ") top(" << _TopIndex << ")" << endl; 
   cout << "******************************************" << endl;
}

MicroCompiler::MicroCompiler(const string &grammer, const string &program):_GrammerFile(grammer),_ProgramFile(program)
{}

void MicroCompiler::Parse()
{
   Productions productions(GrammerAnalyzer(_GrammerFile).GetProductions());
   Table table(TableGenerator(_GrammerFile).GetTable());
   Scanner scanner(_ProgramFile);
   string remainingInput(scanner.GetRemainingInput());
   scanner.ReadNextToken();
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
         const pair<string,string> key(make_pair(topSymbol, _GetTokenStr(scanner.GetNextToken())));
         const size_t productionNum(table[key]);
            cerr << "Key : " << key.first << ", " << key.second << endl;
         if (productionNum == 0)
         {
            cerr << "Invalid data -- non terminal : " << topSymbol << " token : " << _GetTokenStr(scanner.GetNextToken()) << endl;
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
            action = "Term : " + _ParseStack.front();
            if (topSymbol != "lamda")
            {
               deque<string>::iterator itr = _SemanticStack.end() - _CurrentIndex;
               *itr = "[" + scanner.GetBuffer() + "]";
               remainingInput = scanner.GetRemainingInput();
               scanner.ReadNextToken();
            }
            if(Debug) cout << "Poping for match" << endl;
            _CurrentIndex++;
            _ParseStack.pop_front();
         }
         else if (topSymbol == "EOP")
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
            cout << "Calling : " << _ParseStack.front() << endl;
            _ParseStack.pop_front();
         }
      }
      _PrintBlock(action, remainingInput);
   }
}
