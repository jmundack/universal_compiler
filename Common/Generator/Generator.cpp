#include "Generator.h"
#include <boost/lexical_cast.hpp>

using namespace std;

ExpressionRec::ExpressionRec(const ExpressionKind kind,const string &s):
   _Kind(kind),
   _Str(s)
{}

ExpressionRec::ExpressionRec(const ExpressionKind kind,const int i):
   _Kind(kind),
   _Str(boost::lexical_cast<string>(i))
{}

string OperationRec::GetStr() const
{
   string s = "Invalid";
   switch(_Token)
   {
      case PlusOp:
         s = "ADD";
         break;
      case MinusOp:
         s = "SUB";
         break;
      default:
         break;
   }
   return s;
}

Generator::Generator(const string &filename):_TempCount(0), _Filename(filename)
{
   remove(_Filename.c_str());
}

void Generator::Start()
{
   _TempCount = 0;
   _SymbolTable.Clear();
   _OutFile.open(_Filename.c_str());
   cout << "Oped output file : " << _Filename << endl;
}

void Generator::Finish()
{
   _Generate("Halt");
}


void Generator::Assign(ExpressionRec source, ExpressionRec target)
{
   _Generate("Store", source.GetStr(), target.GetName());
}

void Generator::ProcessID(const string &name, ExpressionRec &e)
{
   _CheckID(name);
   e = ExpressionRec(ID, name);
}

void Generator::ProcessLiteral(const string &name, ExpressionRec &e)
{
   e = ExpressionRec(Literal, name);
}

void Generator::ProcessOperationRec(const Token token, OperationRec &op)
{
   cout << "ProcessOperationRec : " << token << endl;
   op = OperationRec(token);
}

void Generator::ReadID(ExpressionRec e)
{
   _Generate("Read", e.GetName(), "Integer");
}

void Generator::WriteID(ExpressionRec e)
{
   _Generate("Write", e.GetStr(), "Integer");
}

ExpressionRec Generator::GenerateInFix(ExpressionRec e1, OperationRec op, ExpressionRec e2)
{
   ExpressionRec result(Temp, _GetTemp());
   _Generate(op.GetStr(), e1.GetStr(), e2.GetStr(), result.GetName());
   return result;
}

void Generator::_Generate(const string &s1,
                          const string &s2,
                          const string &s3,
                          const string &s4)
{
   if (!_OutFile.good())
   {
      cerr << "Cant write to file : " << _Filename << endl;
      return;
   }
   _OutFile << s1;
   if (!s2.empty()) _OutFile << " " << s2;
   if (!s3.empty()) _OutFile << ", " << s3;
   if (!s4.empty()) _OutFile << ", " << s4;
   _OutFile << endl;

   _CodeGenerated << s1;
   if (!s2.empty()) _CodeGenerated << " " << s2;
   if (!s3.empty()) _CodeGenerated << ", " << s3;
   if (!s4.empty()) _CodeGenerated << ", " << s4;
   _CodeGenerated << endl;
}

void Generator::_CheckID(const string &symbol)
{
   if (!_SymbolTable.Exists(symbol))
   {
      _SymbolTable.Add(symbol);
      _Generate("Declare", symbol, "Integer");
   }
}

string Generator::_GetTemp()
{
   _TempCount ++;
   string s = "Temp&" + boost::lexical_cast<string>(_TempCount);
   _CheckID(s);
   return s;
}
