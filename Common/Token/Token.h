#pragma once 
#include <deque>
#include <iostream>


enum Token          // enums representing all the vlaid tokens
{
    Invalid,
    BeginSym, 
    EndSym, 
    ReadSym, 
    WriteSym, 
    Id, 
    IntLiteral, 
    LParen, 
    RParen, 
    SemiColon, 
    Comma, 
    AssignOp, 
    PlusOp, 
    MinusOp, 
    EofSym,
    Max
};

/**
 * \brief operator over load for Token enum to help output the tokens
 */
std::ostream& operator<< (std::ostream &out, const Token token);

typedef std::deque<Token> Tokens;
