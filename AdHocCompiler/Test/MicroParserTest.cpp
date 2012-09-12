#include <iostream>
#include "MicroParser.h"

using namespace std;

int main ( int argc, char *argv[] )
{
  // validate arguments
  if (argc != 2)
  {
    cerr << "Usage : " << argv[0] << " <file to parse>" << endl;
    return 1;
  }
  MicroParser microParser(argv[1]);
  microParser.Parse();
  return 0;
}
