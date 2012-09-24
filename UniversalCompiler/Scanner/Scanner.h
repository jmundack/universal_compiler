#pragma once
#include <fstream>
#include <string>
#include "Token.h"

enum State
{
   StateInvalid = -1,
   Start,
   State1,
   State2,
   State3,
   State4,
   State5,
   State6,

   MinFinalState = 10,
   StateID,
   StateIntLiteral,
   StateEmptySpace,
   StatePlusOp,
   StateComment,
   StateAssignOp,
   StateComma,
   StateSemiColon,
   StateLParen,
   StateRParen,
   StateMinusOp,
   MaxFinalState,
   StateMax
};

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
   Tokens GetTokens();

   Token GetNextToken() const { return _NextToken; }
   void ReadNextToken();
   const std::string &GetBuffer() const { return _Buffer; }
private:
   /**
    * \brief Helper function to convert a string to a token
    * NOTE : This function can handle upper and lower case token strings
    */
   Token _LookUpCode(const State currentState, const std::string &s) const;  

   char _GetNextChar();
   void _ConsumeChar();
   State _NextState(const State currentState, bool &consumeChar);
   bool _IsValidChar(const char c) const;
   bool _IsEmptyChar(const char c) const;
   bool _IsStateFinal(const State state) const;

   std::string _Filename;  /**< name of the file to be scanned */
   std::ifstream _InFile;
   std::string _Buffer;
   Token _NextToken;

};
