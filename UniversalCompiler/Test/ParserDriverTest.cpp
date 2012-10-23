#include <iostream>
#include "ParserDriver.h"

using namespace std;

int main ( int argc, char *argv[] )
{
  // validate arguments
  if (argc != 2)
  {
    cerr << "Usage : " << argv[0] << " <file to parse>" << endl;
    return 1;
  }
  ParserDriver p(argv[1]);
  p.Parse();
  return 0;
}
