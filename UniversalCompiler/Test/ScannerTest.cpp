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
  Tokens tokens(s.GetTokens());
  for (Tokens::const_iterator itr = tokens.begin(); itr != tokens.end(); ++itr)
    cout << *itr;
  cout << endl;
  return 0;
}
