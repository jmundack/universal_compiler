#include "GrammerAnalyzer.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <set>

using namespace std;

GrammerAnalyzer::GrammerAnalyzer(const string &filename):_Filename(filename){}

void GrammerAnalyzer::Analyze()
{
   typedef vector<pair<string, string> > Productions;
   typedef set<string> Symbols;
   Symbols terminalSymbols;
   Symbols nonTerminalSymbols;
   Productions productions;
   ifstream inFile(_Filename.c_str());
   while (inFile.good() && !inFile.eof())
   {
      string line;
      getline(inFile, line);
      if (line.empty()) continue;
      {
         const string lhs = line.substr(0,line.find_first_of('>')+1);
         const string rhs = line.substr(line.find_first_of('>')+1);
         productions.push_back(make_pair(lhs,rhs));
      }
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
            nonTerminalSymbols.insert(word);
         else
            terminalSymbols.insert(word);
         line = line.substr(word.size());
      }
   }

   cout << "************************" << endl;
   cout << "Productions : " << endl;
   for (Productions::const_iterator itr = productions.begin(); itr != productions.end(); ++itr)
      cout << (*itr).first << " \t-->\t " << (*itr).second << endl;
   cout << "************************" << endl;
   cout << "Terminal Symbols : " << endl;
   for (Symbols::const_iterator itr = terminalSymbols.begin(); itr != terminalSymbols.end(); ++itr)
      cout << (*itr) << endl;
   cout << "************************" << endl;
   cout << "Non Terminal Symbols : " << endl;
   for (Symbols::const_iterator itr = nonTerminalSymbols.begin(); itr != nonTerminalSymbols.end(); ++itr)
      cout << (*itr) << endl;
}
