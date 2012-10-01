#include <iostream>
#include "GrammerAnalyzer.h"

using namespace std;

int main ( int argc, char *argv[] )
{
  // validate arguments
  if (argc != 2)
  {
    cerr << "Usage : " << argv[0] << " <file to parse>" << endl;
    return 1;
  }
  GrammerAnalyzer g(argv[1]);
  g.Analyze();
  return 0;
}
