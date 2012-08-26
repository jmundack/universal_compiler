#include <string>
#include <vector>

enum Token
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

std::ostream& operator<< (std::ostream &out, const Token token);


class Scanner
{
public:
   Scanner(const std::string &filename);
   std::vector<Token> GetTokens() const;
private:
   Token _GetTokenForString(const std::string &s) const;
   std::string _Filename;
};
