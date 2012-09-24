#include "Scanner.h"

#include <iostream>
#include <locale>
#include <fstream>
#include <algorithm>

using namespace std;

std::ostream& operator<< (std::ostream &out, const State state)
{
   switch (state)
   {
      case StateInvalid:
         out << "StateInvalid";
         break;
      case Start:
         out << "Start";
         break;
      case State1:
         out << "State1";
         break;
      case State2:
         out << "State2";
         break;
      case State3:
         out << "State3";
         break;
      case State4:
         out << "State4";
         break;
      case State5:
         out << "State5";
         break;
      case State6:
         out << "State6";
         break;
      case MinFinalState:
         out << "MinFinalState";
         break;
      case StateID:
         out << "StateID";
         break;
      case StateIntLiteral:
         out << "StateIntLiteral";
         break;
      case StateEmptySpace:
         out << "StateEmptySpace";
         break;
      case StatePlusOp:
         out << "StatePlusOp";
         break;
      case StateComment:
         out << "StateComment";
         break;
      case StateAssignOp:
         out << "StateAssignOp";
         break;
      case StateComma:
         out << "StateComma";
         break;
      case StateSemiColon:
         out << "StateSemiColon";
         break;
      case StateLParen:
         out << "StateLParen";
         break;
      case StateRParen:
         out << "StateRParen";
         break;
      case StateMinusOp:
         out << "StateMinusOp";
         break;
      case MaxFinalState:
         out << "MaxFinalState";
         break;
      case StateMax:
         out << "StateMax";
         break;
      default:
         out << "Unknown";
   }
   return out;
}

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
   State state = Start;
   _Buffer.clear();
   _NextToken = Invalid;
   bool done(false);
   while(!done && _InFile.good() && !_InFile.eof())
   {
      if (_IsStateFinal(state))
      {
         _NextToken = _LookUpCode(state,_Buffer);
         if (_NextToken != Invalid)
            done = true;
         else
         {
            cout << "state : " << state << " is dont care state so setting to start again " << endl;
            state = Start;
            _Buffer.clear();
         }
      }
      else
      {
         bool consumeChar(false);
         state = _NextState(state, consumeChar);
         if (!_IsEmptyChar(_GetNextChar()) && consumeChar)
            _Buffer += _GetNextChar();
         if (consumeChar) _ConsumeChar();
      }
   }
   if (_InFile.eof())
      _NextToken = EofSym;
   cout << "Token : " << _NextToken << " buffer --> \"" << _Buffer << '"' << endl;
}

State Scanner::_NextState(const State currentState, bool &consumeChar)
{
   State nextState = StateInvalid;
   const char c = _GetNextChar();
   consumeChar = true;
   switch (currentState)
   {
      case Start:
         if (isalpha(c))
            nextState = State1;
         else if (isdigit(c))
            nextState = State2;
         else if (c == '+')
         {
            nextState = StatePlusOp;
         }
         else if (c == '-')
            nextState = State4;
         else if (c == ':')
            nextState = State6;
         else if (c == ',')
         {
            nextState = StateComma;
         }
         else if (c == ';')
         {
            nextState = StateSemiColon;
         }
         else if (c == '(')
         {
            nextState = StateLParen;
         }
         else if (c == ')')
         {
            nextState = StateRParen;
         }
         else if (_IsEmptyChar(c))
            nextState = StateEmptySpace;
         break;
      case State1:
         if (isalnum(c) || c == '_')
            nextState = State1;
         else if (_IsValidChar(c))
         {
            nextState = StateID;
            if (!_IsEmptyChar(c)) consumeChar = false;
         }
         break;
      case State2:
         if (isdigit(c))
            nextState = State2;
         else if (_IsValidChar(c))
         {
            nextState = StateIntLiteral;
            if (!_IsEmptyChar(c)) consumeChar = false;
         }
         break;
      case State3:
         if (_IsEmptyChar(c))
            nextState = State3;
         else if (_IsValidChar(c))
            nextState = StateEmptySpace;
         break;
      case State4:
         if (c == '-')
            nextState = State5;
         else if (_IsValidChar(c) && c != '_')
         {
            nextState = StateMinusOp;
            if (!_IsEmptyChar(c)) consumeChar = false;
         }
         break;
      case State5:
         if (c == '\n')
            nextState = StateComment;
         else 
            nextState = State5;
         break;
      case State6:
         if (c == '=')
            nextState = StateAssignOp;
         break;

      case StateID:
      case StateIntLiteral:
      case StateEmptySpace:
      case StatePlusOp:
      case StateComment:
      case StateAssignOp:
      case StateComma:
      case StateSemiColon:
      case StateLParen:
      case StateRParen:
      case StateMinusOp:
         nextState = Start;
         break;
      default:
         break;
   }
   return nextState;
}

Token Scanner::_LookUpCode(const State state, const std::string &s) const
{
  Token token = Invalid;
  if (state <= MinFinalState && state >= MaxFinalState)
     return token;
  if (state == StateID)
  {
     if (s == "begin")   token = BeginSym;
     else if (s == "end")   token = EndSym;
     else if (s == "write")   token = WriteSym;
     else if (s == "read")   token = ReadSym;
     else 
        token = Id;
  }
  else if (state == StateIntLiteral)
     token = IntLiteral;
  else if (state == StatePlusOp)
     token = PlusOp;
  else if (state == StateAssignOp)
     token = AssignOp;
  else if (state == StateComma)
     token = Comma;
  else if (state == StateSemiColon)
     token = SemiColon;
  else if (state == StateLParen)
     token = LParen;
  else if (state == StateRParen)
     token = RParen;
  else if (state == StateMinusOp)
     token = MinusOp;
#if DEBUG
  cout << "*** returning " << token << " for str : \"" << s << "\" and state : " << state << endl;
#endif
  return token;
}


bool Scanner::_IsValidChar(const char c) const
{
   bool isValid(false);
   if (isalnum(c))
      isValid = true;
   else if (c == '+')
      isValid = true;
   else if (c == '-')
      isValid = true;
   else if (c == '=')
      isValid = true;
   else if (c == ':')
      isValid = true;
   else if (c == ',')
      isValid = true;
   else if (c == ';')
      isValid = true;
   else if (c == '(')
      isValid = true;
   else if (c == ')')
      isValid = true;
   else if (c == '\t')
      isValid = true;
   else if (c == '_')
      isValid = true;
   else if (c == '\n')
      isValid = true;
   else if (c == ' ')
      isValid = true;
   return isValid;
}

bool Scanner::_IsEmptyChar(const char c) const
{
   return (c == ' ' || c == '\t' || c == '\n');
}

bool Scanner::_IsStateFinal(const State state) const
{
   const bool isFinal((state > MinFinalState && state < MaxFinalState) );
   return isFinal;
}

char Scanner::_GetNextChar()
{
   return _InFile.peek();
}

void Scanner::_ConsumeChar()
{
   _InFile.get();
}


