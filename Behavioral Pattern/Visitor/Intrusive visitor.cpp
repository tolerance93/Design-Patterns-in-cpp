// visitor examples for design patterns c++ book
#include <sstream>
#include <string>
#include <iostream>
using namespace std;

// OCP원칙에 위배된다. 이미 작성한 코드의 Expression 인터페이스를 직접적으로 수정하는 방법
// 상속 관계에 따라 모든 하위 클래스도 수정된다.
// 이러한 수정은 클래스 계층에 대한 소스 코드 수정이 가능하다는 것을 전제로 하고 있다.

struct Expression
{
  virtual void print(ostringstream& oss) = 0;
};

struct DoubleExpression : Expression
{
  double value;
  explicit DoubleExpression(const double value)
    : value{value} {}

  void print(ostringstream& oss) override
  {
    oss << value;
  }
};

struct AdditionExpression : Expression
{
  Expression *left, *right;

  AdditionExpression(Expression* const left, Expression* const right)
    : left{left}, right{right} {}

  ~AdditionExpression()
  {
    delete left;
    delete right;
  }

  void print(ostringstream& oss) override
  {
    oss << "(";
    left->print(oss);
    oss << "+";
    right->print(oss);
    oss << ")";
  }
};

void main_2_()
{
  auto e = new AdditionExpression{
    new DoubleExpression{1},
    new AdditionExpression{
      new DoubleExpression{2},
      new DoubleExpression{3}
    }
  };
  ostringstream oss;
  e->print(oss);
  cout << oss.str() << endl;
}
