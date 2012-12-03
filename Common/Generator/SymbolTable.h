#include <vector>
#include <string>
#include <map>

class SymbolTable
{
public:
   SymbolTable():_ScopeNum(0){}
   void Begin();
   void End();
   void Print();
   bool Exists(const std::string &symbol);
   void Add(const std::string &newSymbol);
   void Clear() { _Symbols.clear(); }
private:
   size_t _ScopeNum;
   typedef std::vector<std::pair<size_t, size_t> > ScopedSymbols;
   std::map<size_t, ScopedSymbols> _Symbols;
   std::vector<size_t> _OpenScopes;
   std::string _Data;
};


