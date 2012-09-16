#pragma once
#include <fstream>
#include <string>
#include "Token.h"

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
   Token _GetTokenForString(const std::string &s) const;  

   std::string _Filename;  /**< name of the file to be scanned */
   std::ifstream _InFile;
   std::string _Buffer;
   Token _NextToken;
};
