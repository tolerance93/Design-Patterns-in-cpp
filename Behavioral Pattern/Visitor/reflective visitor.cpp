// visitor examples for design patterns c++ book
#include <sstream>
#include <string>
#include <iostream>
using namespace std;

struct Expression
{
    // virtual 소멸자를 두어 다형성을 가질 수 있도록 한다.
  virtual ~Expression() = default;
};

struct DoubleExpression : Expression
{
  double value;
  explicit DoubleExpression(const double value)
    : value{ value } {}
};

struct AdditionExpression : Expression
{
  Expression *left, *right;

  AdditionExpression(Expression* const left, Expression* const right)
    : left{ left }, right{ right } {}

  ~AdditionExpression()
  {
    delete left;
    delete right;
  }
};

struct ExpressionPrinter
{
  /*void print(DoubleExpression *de, ostringstream& oss) const
  {
    oss << de->value;
  }
  void print(AdditionExpression *ae, ostringstream& oss) const
  {
    oss << "(";
    print(ae->left, oss);
    oss << "+";
    print(ae->right, oss);
    oss << ")";
  }*/
  ostringstream oss;

    // 이러한 접근법은 모든 클래스 계층마다 구현해 넣은 print()메서드에 대한 컴파일러의 타입 체크를 포기해야 한다.
    // if chain을 신경써야. dynamic cast 비용이 크지는 않지만 존재는 함.
  void print(Expression *e)
  {
    if (auto de = dynamic_cast<DoubleExpression*>(e))
    {
      oss << de->value;
    } 
    else if (auto ae = dynamic_cast<AdditionExpression*>(e))
    {
      oss << "(";
      print(ae->left);
      oss << "+";
      print(ae->right);
      oss << ")";
    }
  }

  string str() const { return oss.str(); }
};

void main_3_()
{
  auto e = new AdditionExpression{
    new DoubleExpression{ 1 },
    new AdditionExpression{
      new DoubleExpression{ 2 },
      new DoubleExpression{ 3 }
    }
  };
  ostringstream oss;
  //e->print(oss);
  ExpressionPrinter ep;
  ep.print(e);
  cout << ep.str() << endl;
}
