#include <iostream>
#include "Scanner.h"

using namespace std;

int main ( int argc, char *argv[] )
{
  // validate arguments
  if (argc != 2)
  {
    cerr << "Usage : " << argv[0] << " <file to parse>" << endl;
    return 1;
  }
  Scanner s(argv[1]);
  vector<Token> tokens(s.GetTokens());
  for (size_t i = 0; i < tokens.size(); i++)
    cout << tokens.at(i) << endl;
  return 0;
}
