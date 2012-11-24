#include "ParserDriverWithSemanticStack.h"
#include "TableGenerator.h"
#include "Scanner.h"
#include "PredictGenerator.h"
#include <stack>
#include <sstream>

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

   void _PrintLine(const string &action, const string &remainingInput, stack<string> s)
   {
      cout << action << "\t" << remainingInput << "\t";
      while(!s.empty())
      {
         cout << s.top() << ' ';
         s.pop();
      }
      cout << endl;
   }
}

ParserDriverWithSemanticStack::ParserDriverWithSemanticStack(const string &grammer, const string &program):_GrammerFile(grammer),_ProgramFile(program)
{}

void ParserDriverWithSemanticStack::Parse()
{
   Productions productions(GrammerAnalyzer(_GrammerFile).GetProductions());
   Table table(TableGenerator(_GrammerFile).GetTable());
   Scanner scanner(_ProgramFile);
   string remainingInput(scanner.GetRemainingInput());
   scanner.ReadNextToken();
   PredictGenerator p(_GrammerFile);
   p.Print();

   stack<string> s;
   s.push("<system_goal>");
   cout << "***********************************************" << endl;
   cout << "Parser Action \t Remaining Input \t ParserStack" << endl;
   while(!s.empty())
   {
      string action;
      if (s.top().at(0) == '<') 
      {
         const pair<string,string> key(make_pair(s.top(), _GetTokenStr(scanner.GetNextToken())));
         const size_t productionNum(table[key]);
         stringstream s;
         s << "Predict : " << productionNum;
         action = s.str();
      }
      else action = "Match\t";
      _PrintLine(action, remainingInput, s);
      if (s.top().at(0) == '<')
      {
         const pair<string,string> key(make_pair(s.top(), _GetTokenStr(scanner.GetNextToken())));
         const size_t productionNum(table[key]);
         if (productionNum == 0)
         {
            cerr << "Invalid data -- non terminal : " << s.top() << " token : " << _GetTokenStr(scanner.GetNextToken()) << endl;
            return;
         }
         else
         {
            s.pop();
            if(Debug) cout << "************** Getting production : " << productionNum << " of " << (productions.size() + 1) << endl;
            const pair<string, vector<string> > &production(productions.at(productionNum -1));
            if(Debug) cout << "************* Getting rhs of production" << endl;
            const vector<string> &rhs(production.second);
            for(vector<string>::const_reverse_iterator itr = rhs.rbegin(); itr != rhs.rend(); ++itr)
               s.push(*itr);
         }
      }
      else
      {
         if (s.top() != "lamda") 
         {
            remainingInput = scanner.GetRemainingInput();
            scanner.ReadNextToken();
         }
         if(Debug) cout << "Poping for match" << endl;
         s.pop();
      }
   }
}
