#pragma once
#include <string>

class ParserDriver
{
public:
   ParserDriver(const std::string &filename);
   void Parse();
private:
   std::string _Filename;
};
