#include <vector>
#include <string>

class SymbolTable
{
public:
   bool Exists(const std::string &symbol) const;
   void Add(const std::string &newSymbol);
   void Clear() { _Symbols.clear(); }
private:
   std::vector<std::string> _Symbols;
};


