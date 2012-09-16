#pragma once 

#include "Token.h"
#include <vector>
#include <fstream>

enum ExpressionKind
{
   ID,
   Literal,
   Temp
};

class ExpressionRec
{
public:
   ExpressionRec(){}
   ExpressionRec(const ExpressionKind kind,const std::string &s);
   ExpressionRec(const ExpressionKind kind,const int i);
   const std::string &GetName() const { return _Str; }
   const std::string &GetStr() const { return _Str; }
private:
   ExpressionKind _Kind;
   std::string _Str;
};

class OperationRec
{
public:
   OperationRec(){}
   OperationRec(const Token token):_Token(token){}
   std::string GetStr() const;
private:
   Token _Token;
};

class SymbolTable
{
public:
   bool Exists(const std::string &symbol) const;
   void Add(const std::string &newSymbol);
   void Clear() { _Symbols.clear(); }
private:
   std::vector<std::string> _Symbols;
};

class Generator
{
public:
   Generator(const std::string &filename);
   void Start();
   void Finish();

   void Assign(ExpressionRec source, ExpressionRec target);
   void ProcessID(const std::string &name, ExpressionRec &e);
   void ProcessLiteral(const std::string &name, ExpressionRec &e);
   void ProcessOperationRec(const Token token, OperationRec &op);
   void ReadID(ExpressionRec e);
   void WriteID(ExpressionRec e);
   ExpressionRec GenerateInFix(ExpressionRec e1, OperationRec op, ExpressionRec e2);
private:

   void _Generate(const std::string &s1,
                  const std::string &s2 = "",
                  const std::string &s3 = "",
                  const std::string &s4 = "");
   void _CheckID(const std::string &symbol);
   std::string _GetTemp();

   size_t _TempCount;
   SymbolTable _SymbolTable;

   std::string _Filename;
   std::ofstream _OutFile;
};
