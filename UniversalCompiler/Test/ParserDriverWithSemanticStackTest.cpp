#include <iostream>
#include "ParserDriverWithSemanticStack.h"

using namespace std;

int main ( int argc, char *argv[] )
{
  // validate arguments
  if (argc != 3)
  {
    cerr << "Usage : " << argv[0] << " <grammer> <program>" << endl;
    return 1;
  }
  ParserDriverWithSemanticStack p(argv[1], argv[2]);
  p.Parse();
  return 0;
}
