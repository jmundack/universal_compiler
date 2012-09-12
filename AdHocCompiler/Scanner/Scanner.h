#include <string>
#include <deque>

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

/**
 * \brief Class generated tokens from an input file.
 */
class Scanner
{
public:
  /**
   * \brief Constructor initializes the scanner with a file name
   */
   Scanner(const std::string &filename);

   /**
    * \brief Returns a vector of all the tokens scanned from the file
    *        passed in.
    */
   Tokens GetTokens() const;
private:
   /**
    * \brief Helper function to convert a string to a token
    * NOTE : This function can handle upper and lower case token strings
    */
   Token _GetTokenForString(const std::string &s) const;  

   std::string _Filename;  /**< name of the file to be scanned */
};
