#include "GrammerAnalyzer.h"
#include <fstream>
#include <iostream>

using namespace std;

GrammerAnalyzer::GrammerAnalyzer(const string &filename):_Filename(filename){}

void GrammerAnalyzer::Analyze()
{
   _TerminalSymbols.clear();
   _NonTerminalSymbols.clear();
   _Productions.clear();
   Symbols lhsSymbols;
   Symbols rhsSymbols;
   ifstream inFile(_Filename.c_str());
   while (inFile.good() && !inFile.eof())
   {
      string line;
      getline(inFile, line);
      if (line.empty()) continue;
      const string lhs = line.substr(0,line.find_first_of('>')+1);
      vector<string> rhs;
      while (!line.empty())
      {
         string word;
         if (line.at(0) == ' ') 
         {
            line = line.substr(1); 
            continue; 
         }
         if (line.at(0) == '<')
         {
            size_t p2 = line.find_first_of('>') + 1;
            word = line.substr(0,p2);
         }
         else
         {
            const bool alphabetsOnly(isalpha(line.at(0)));
            for (string::const_iterator itr = line.begin(); itr != line.end(); ++itr)
            {
               if (*itr == ' ') break;
               if (alphabetsOnly)
               {
                  if (isalpha(*itr))
                     word += *itr;
                  else
                     break;
               }
               else
               {
                  if (*itr == '<')
                     break;
                  word += *itr;
               }
            }
         }
         cout << "Processing Word : " << word << endl;
         if (word.at(0) == '<')
            _NonTerminalSymbols.insert(word);
         else
            _TerminalSymbols.insert(word);
         rhs.push_back(word);
         line = line.substr(word.size());
      }
      rhs.erase(rhs.begin());
      _Productions.push_back(make_pair(lhs,rhs));
      lhsSymbols.insert(lhs);
      rhsSymbols.insert(rhs.begin(), rhs.end());
   }

   cout << "************************" << endl;
   cout << "Productions : " << endl;
   for (Productions::const_iterator itr = _Productions.begin(); itr != _Productions.end(); ++itr)
   {
      cout << (*itr).first << " \t-->\t " ;
      for (vector<string>::const_iterator itr2 = (*itr).second.begin(); itr2 != (*itr).second.end(); ++itr2)
         cout << *itr2;
      cout << endl;
   }
   cout << "************************" << endl;
   cout << "Terminal Symbols : " << endl;
   for (Symbols::const_iterator itr = _TerminalSymbols.begin(); itr != _TerminalSymbols.end(); ++itr)
      cout << (*itr) << endl;
   cout << "************************" << endl;
   cout << "Non Terminal Symbols : " << endl;
   for (Symbols::const_iterator itr = _NonTerminalSymbols.begin(); itr != _NonTerminalSymbols.end(); ++itr)
      cout << (*itr) << endl;

   cout << "************************" << endl;
   cout << "Non Terminal Symbols On LHS of Productions : " << endl;
   for (Symbols::const_iterator itr = lhsSymbols.begin(); itr != lhsSymbols.end(); ++itr)
      cout << (*itr) << endl;

   cout << "************************" << endl;
   cout << "Symbols on RHS of productions : " << endl;
   for (Symbols::const_iterator itr = rhsSymbols.begin(); itr != rhsSymbols.end(); ++itr)
      cout << (*itr) << endl;
}
