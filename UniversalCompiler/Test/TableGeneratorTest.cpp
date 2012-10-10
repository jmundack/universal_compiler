#include <iostream>
#include "TableGenerator.h"

using namespace std;

int main ( int argc, char *argv[] )
{
  // validate arguments
  if (argc != 2)
  {
    cerr << "Usage : " << argv[0] << " <file to parse>" << endl;
    return 1;
  }
  TableGenerator t(argv[1]);
  t.GenerateTable();
  return 0;
}
