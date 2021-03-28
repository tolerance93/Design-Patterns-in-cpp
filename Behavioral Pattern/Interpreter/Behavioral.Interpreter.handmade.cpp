#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <sstream>
#include <memory>
using namespace std;
#include <boost/lexical_cast.hpp>

/**
 * 뭔가 빠뜨린 것이 있을 때 렉서나 파서는 무언가 의미 있는 것을 만날 때까지 잘못된 코드를 그냥 넘겨버린다.
 * 특히 정적 도구들이 이러한 접근법을 취한다. 그렇게 해야만 실시간으로 타이핑하고 있는 미완성 코드에서도 의미 있게 동작할 수 있다.
 */

// lexing =================================================
/**
 * 문자열 입력을 토큰이라 불리는 단위로 나누어 나열한다. 토큰은 어떤 문법상에서 의미를 가지는 최소 단위.
 * 이 경우에 정수, 연산자, 괄호로 나뉨
 */

struct Token
{
  enum Type { integer, plus, minus, lparen, rparen } type;
    //연산자와 괄호 기호는 분류되는 순간 값도 정해지지만, 숫자를 위해 string필요.
  string text;

  explicit Token(Type type, const string& text) :
    type{type}, text{text} {}

  friend ostream& operator<<(ostream& os, const Token& obj)
  {
    return os << "`" << obj.text << "`";
  }
};

vector<Token> lex(const string& input)
{
  vector<Token> result;

  for (int i = 0; i < input.size(); ++i)
  {
    switch (input[i])
    {
    case '+':
      result.push_back(Token{ Token::plus, "+" });
      break;
    case '-':
      result.push_back(Token{ Token::minus, "-" });
      break;
    case '(':
      result.push_back(Token{ Token::lparen, "(" });
      break;
    case ')':
      result.push_back(Token{ Token::rparen, ")" });
      break;
    default:
      // number
      //숫자를 읽는 동안 버퍼에 쌓고 더이상 숫자가 읽히지 않으면 작업을 종료하고 저장된 버퍼 전체를 토큰으로하여 결과 벡터에 저장.
      ostringstream buffer;
      buffer << input[i];
      for (int j = i + 1; j < input.size(); ++j)
      {
        if (isdigit(input[j]))
        {
          buffer << input[j];
          ++i;
        }
        else
        {
          result.push_back(Token{ Token::integer, buffer.str() });
          break;
        }
      }
      if (!buffer.str().empty()) // 20201210
        result.push_back(Token{ Token::integer, buffer.str() });
    }
  }

  return result;
}

// parsing =====================================================
/**
 * 파싱은 토큰의 나열을 의미 있는 단위로 바꾼다.
 * 토큰 정의 트리에서 최상단에 추상 부모 타입을 두면 편리하다.
 */

struct Element
{
  virtual ~Element() = default;
  virtual int eval() const = 0;
};

struct Integer : Element
{
  int value;
  explicit Integer(const int value)
    : value(value)
  {
  }
  int eval() const override { return value; }
};

struct BinaryOperation : Element
{
  enum Type { addition, subtraction } type;
  shared_ptr<Element> lhs, rhs;

  int eval() const override
  {
    if (type == addition) 
      return lhs->eval() + rhs->eval();
    return lhs->eval() - rhs->eval();
  }
};

/**
 * 트리를 만드는 코드.
 * 숫자의 경우 표현식의 왼쪽에 위치해야 할지 오른쪽에 위치해야 할 지 알 수 가 없다.  have_lhs변수를 두어 어느쪽에 위치해야하는지 기록을 두어 활용.
 */
unique_ptr<Element> parse(const vector<Token>& tokens)
{
  auto result = make_unique<BinaryOperation>();
  bool have_lhs = false;
  for (size_t i = 0; i < tokens.size(); i++)
  {
    auto token = tokens[i];
    switch(token.type)
    {
    case Token::integer:
    {
      int value = boost::lexical_cast<int>(token.text);
      auto integer = make_shared<Integer>(value);
      if (!have_lhs) {
        result->lhs = integer;
        have_lhs = true;
      }
      else result->rhs = integer;
    }
      break;
    case Token::plus: 
      result->type = BinaryOperation::addition;
      break;
    case Token::minus:
      result->type = BinaryOperation::subtraction;
      break;
    case Token::lparen: 
    {
      int j = i;
      for (; j < tokens.size(); ++j)
        if (tokens[j].type == Token::rparen)
          break; // found it!

      vector<Token> subexpression(&tokens[i + 1], &tokens[j]);
      auto element = parse(subexpression);
      if (!have_lhs) 
      {
        result->lhs = move(element);
        have_lhs = true;
      }
      else result->rhs = move(element);
      i = j; // advance
    }
    break;
    }
  }
  return result;
}



int main()
{
  string input{ "(14-4)+11" }; // see if you can make nested braces work
  auto tokens = lex(input);

  // let's see the tokens
  for (auto& t : tokens)
    cout << t << "   ";
  cout << endl;

  try {
    auto parsed = parse(tokens);
    cout << input << " = " << parsed->eval() << endl;
  } 
  catch (const exception& e)
  {
    cout << e.what() << endl;
  }

  getchar();
  return 0;
}
