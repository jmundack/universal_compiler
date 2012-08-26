#include "Scanner.h"

#include <iostream>

using namespace std;

Scanner::Scanner(const string &filename):_Filename(filename) 
{
   cout << "Scanner initialized for : " << _Filename << endl;
}
