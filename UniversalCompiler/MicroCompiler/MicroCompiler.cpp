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

   void _PrintBlock(const string &action, const string &remainingInput, const deque<string> &ps, const deque<string>& ss,
         const size_t leftIndex,
         const size_t rightIndex,
         const size_t currentIndex,
         const size_t topIndex)
   {
      cout << "******************************************" << endl;
      cout << action << endl;
      cout << "Input : " << remainingInput << endl;
      cout << "PS : ";
      std::copy(ps.begin(), ps.end(), std::ostream_iterator<string>(std::cout, ""));
      cout << endl;
      cout << "SS : ";
      std::copy(ss.begin(), ss.end(), std::ostream_iterator<string>(std::cout, ""));
      cout << endl;
      cout << "Indices : Left(" << leftIndex << ") right (" << rightIndex << ") current(" << currentIndex << ") top(" << topIndex << ")" << endl; 
      cout << "******************************************" << endl;
   }
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

   deque<string> parseStack, semanticStack;
   parseStack.push_front("<system_goal>");
   semanticStack.push_front("<system_goal>");
   size_t leftIndex(0), rightIndex(0), currentIndex(1), topIndex(2);

   stack<string> s;
   s.push("<system_goal>");
   cout << "***********************************************" << endl;
   _PrintBlock("Enrty", remainingInput, parseStack, semanticStack, leftIndex, rightIndex, currentIndex, topIndex);
   while(!parseStack.empty())
   {
      string action("INVALID");
      string topSymbol = parseStack.at(0);
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
            parseStack.pop_front();
            parseStack.push_front(boost::lexical_cast<string>(topIndex));
            parseStack.push_front(boost::lexical_cast<string>(currentIndex));
            parseStack.push_front(boost::lexical_cast<string>(rightIndex));
            parseStack.push_front(boost::lexical_cast<string>(leftIndex));
            parseStack.push_front("EOP");
            if(Debug) cout << "************** Getting production : " << productionNum << " of " << (productions.size() + 1) << endl;
            const pair<string, vector<string> > &production(productions.at(productionNum -1));
            if(Debug) cout << "************* Getting rhs of production" << endl;
            const vector<string> &rhs(production.second);
            for(vector<string>::const_reverse_iterator itr = rhs.rbegin(); itr != rhs.rend(); ++itr)
               parseStack.push_front(*itr);
            size_t numNonActionSymbols(0);
            for(vector<string>::const_iterator itr = rhs.begin(); itr != rhs.end(); ++itr)
               if (itr->at(0) != '#') // skip action symbols
               {
                  numNonActionSymbols++;
                  semanticStack.push_front(*itr);
               }
            leftIndex = currentIndex;
            rightIndex = topIndex;
            currentIndex = rightIndex;
            topIndex = topIndex + numNonActionSymbols;
         }
      }
      else
      {
         const Symbols &terminalSymbols(p.GetTerminalSymbols());
         if (terminalSymbols.find(topSymbol) != terminalSymbols.end() || topSymbol == "lamda")
         {
            action = "Term : " + parseStack.front();
            if (topSymbol != "lamda")
            {
               deque<string>::iterator itr = semanticStack.end() - currentIndex;
               *itr = "[" + scanner.GetBuffer() + "]";
               remainingInput = scanner.GetRemainingInput();
               scanner.ReadNextToken();
            }
            if(Debug) cout << "Poping for match" << endl;
            currentIndex ++;
            parseStack.pop_front();
         }
         else if (topSymbol == "EOP")
         {
            action = "EOP : (" + parseStack.at(1) + ',' + parseStack.at(2) +',' + parseStack.at(3) +','+ parseStack.at(4) + ")";
            parseStack.pop_front(); // pop EOP
            // restore all indices
            leftIndex = boost::lexical_cast<size_t>(parseStack.front());
            parseStack.pop_front();
            rightIndex = boost::lexical_cast<size_t>(parseStack.front());
            parseStack.pop_front();
            currentIndex = boost::lexical_cast<size_t>(parseStack.front());
            parseStack.pop_front();
            topIndex = boost::lexical_cast<size_t>(parseStack.front());
            parseStack.pop_front();
            // Done restoring indices
            currentIndex ++;
            while(semanticStack.size() != topIndex -1)
            {
               cout << "Popping : " << semanticStack.front()  << " from semantic stack " << endl;
               semanticStack.pop_front();
            }
         }
         else
         {
            action = "Action : " + parseStack.front();
            cout << "Calling : " << parseStack.front() << endl;
            parseStack.pop_front();
         }
      }
      _PrintBlock(action, remainingInput, parseStack, semanticStack, leftIndex, rightIndex, currentIndex, topIndex);
   }
}
