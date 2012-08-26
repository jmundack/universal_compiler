#include "Scanner.h"

#include <iostream>
#include <locale>
#include <fstream>

using namespace std;

ostream& operator<< (std::ostream &os , const Token token)
{
  string s;
  switch (token)
  {
    case Invalid : s = "Invalid"; break;
    case BeginSym : s = "BeginSym"; break;
    case EndSym : s = "EndSym"; break;
    case ReadSym : s = "ReadSym"; break;
    case WriteSym : s = "WriteSym"; break;
    case Id : s = "Id"; break;
    case IntLiteral : s = "IntLiteral"; break;
    case LParen : s = "LParen"; break;
    case RParen : s = "RParen"; break;
    case SemiColon : s = "SemiColon"; break;
    case Comma : s = "Comma"; break;
    case AssignOp : s = "AssignOp"; break;
    case PlusOp : s = "PlusOp"; break;
    case MinusOp : s = "MinusOp"; break;
    case EofSym : s = "EofSym"; break;
    case Max : s = "Max"; break;
    default:break;
  }
  os << "<" << s << ">";
  return os;
}

Scanner::Scanner(const string &filename):_Filename(filename) 
{
   cout << "Scanner initialized for : " << _Filename << endl;
}

vector<Token> Scanner::GetTokens() const
{
   vector<Token> tokens;
   ifstream inFile(_Filename.c_str());
   while (inFile.good() && !inFile.eof())
   {
     char c = '*';
     inFile.get(c);
     if (inFile.eof()) break;
     switch (c)
     {
       case '\t': break;
       case '\n': break;
       case ' ': break;
       case '(' : tokens.push_back(LParen); break;
       case ')' : tokens.push_back(RParen); break;
       case ';' : tokens.push_back(SemiColon); break;
       case ',' : tokens.push_back(Comma); break;
       case '+' : tokens.push_back(PlusOp); break;
       default:
       {
         if (isalpha(c))
         {
           string s;
           s += c;
           while (true)
           {
             char nextChar = inFile.peek();
             if (isalnum(nextChar) || nextChar == '_')
             {
               inFile >> nextChar;
               s += nextChar;
             }
             else
               break;
           }
           tokens.push_back(_GetTokenForString(s));
         }
         else if (isdigit(c))
         {
           while (isdigit(inFile.peek()))
           {
             char nextChar;
             inFile.get(nextChar);
           }
           tokens.push_back(IntLiteral);
         }
         else if (c == ':')
         {
           if (inFile.peek() == '=')
           {
             char n;
             inFile.get(n);
             tokens.push_back(AssignOp);
           }
           else
           {
             cerr << ": followed by " << inFile.peek() << " -- hex : " << hex << inFile.peek() << endl;
             tokens.push_back(Invalid);
           }
         }
         else if (c == '-')
         {
           if (inFile.peek() == '-')
           {
             char n = '-';
             while (n != '\n')
               inFile.get(n);

           }
           else
             tokens.push_back(MinusOp);
         }
         else
         {
           cerr << "Unexpected char : "  << c << " -- hex : " << hex << (int)c << endl;
           tokens.push_back(Invalid);
         }
         break;
       }
     }
   }
   if (inFile.eof())
     tokens.push_back(EofSym);

   return tokens;
}

Token Scanner::_GetTokenForString(const std::string &s) const
{
  Token token = Id;
  if (s == "begin")   token = BeginSym;
  else if (s == "end")   token = EndSym;
  else if (s == "write")   token = WriteSym;
  else if (s == "read")   token = ReadSym;
  cout << "*** returning " << token << " for str : " << s << endl;
  return token;
}
