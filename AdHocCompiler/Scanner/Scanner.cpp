#include "Scanner.h"

#include <iostream>
#include <locale>
#include <fstream>
#include <algorithm>

using namespace std;

Scanner::Scanner(const string &filename):_Filename(filename), _InFile(filename.c_str()), _NextToken(Invalid)
{
   cout << "Scanner initialized for : " << _Filename << endl;
}

Tokens Scanner::GetTokens()
{
   Tokens tokens;
   if (!_InFile.good())
   {
     cerr << "Failed to open file : " << _Filename << endl;
     return tokens;
   }
   while (_InFile.good() && !_InFile.eof())
   {
      ReadNextToken();
      tokens.push_back(_NextToken);
   }
   return tokens;
}

void Scanner::ReadNextToken()
{
   Token token = Invalid;
   string s;
READ:
   char c = '*';
   _InFile.get(c);
   if (_InFile.eof())
   {
     token = EofSym;
     s = '$';
   }
   else
   {
      if (c == '\t' || c == '\n' || c == ' ')
        goto READ;
     switch (c)
     {
       case '\t': break;
       case '\n': break;
       case ' ': break;
       case '(' : s = c; token = LParen; break;
       case ')' : s = c; token = RParen; break;
       case ';' : s = c; token = SemiColon; break;
       case ',' : s = c; token = Comma; break;
       case '+' : s = c; token = PlusOp; break;
       default:
       {
         if (isalpha(c))
         {
           s += c;
           while (true)
           {
             char nextChar = _InFile.peek();
             if (isalnum(nextChar) || nextChar == '_')
             {
               _InFile >> nextChar;
               s += nextChar;
             }
             else
               break;
           }
           token = _GetTokenForString(s);
         }
         else if (isdigit(c))
         {
            s = c;
           while (isdigit(_InFile.peek()))
           {
              char nextChar;
              _InFile.get(nextChar);
              s += nextChar;
           }
           if (isalnum(_InFile.peek()))
           {
              while (true)
              {
                 char nextChar = _InFile.peek();
                 if (isalnum(nextChar) || nextChar == '_')
                 {
                    _InFile >> nextChar;
                    s += nextChar;
                 }
                 else
                    break;
              }
              token = Invalid;
           }
           else
              token = IntLiteral;
         }
         else if (c == ':')
         {
            s = c;
           if (_InFile.peek() == '=')
           {
             char n;
             _InFile.get(n);
             s += n;
             token = AssignOp;
           }
           else
           {
             cerr << ": followed by " << _InFile.peek() << " -- hex : " << hex << _InFile.peek() << endl;
             token = Invalid;
           }
         }
         else if (c == '-')
         {
           if (_InFile.peek() == '-')
           {
             char n = '-';
             while (n != '\n')
               _InFile.get(n);
             goto READ;
           }
           else
           {
              s = '-';
              token = MinusOp;
           }
         }
         else
         {
           cerr << "Unexpected char : "  << c << " -- hex : " << hex << (int)c << endl;
           token = Invalid;
         }
         break;
       }
     }
   }
   _Buffer = s;
   _NextToken = token;
   cout << "Token : " << _NextToken << " buffer --> " << _Buffer << endl;
}

Token Scanner::_GetTokenForString(const std::string &s) const
{
  Token token = Id;
  if (s == "begin")   token = BeginSym;
  else if (s == "end")   token = EndSym;
  else if (s == "write")   token = WriteSym;
  else if (s == "read")   token = ReadSym;
#if DEBUG
  cout << "*** returning " << token << " for str : " << s << endl;
#endif
  return token;
}

