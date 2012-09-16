#include "Token.h"

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


