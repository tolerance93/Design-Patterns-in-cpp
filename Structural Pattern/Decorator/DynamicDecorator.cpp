#include <string>
#include <iostream>
#include <vector>
#include <sstream>
using namespace std;
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

struct Shape
{
  virtual string str() const = 0;
};

struct Circle : Shape
{
  float radius;

  Circle(){}

  explicit Circle(const float radius)
    : radius{radius}
  {
  }

    /**
     * resize는 shape에 없기 때문에, 데커레이터에서 호출할 수 없다.
     */
  void resize(float factor)
  {
    radius *= factor;
  }

  string str() const override
  {
    ostringstream oss;
    oss << "A circle of radius " << radius;
    return oss.str();
  }
};

struct Square : Shape
{
  float side;

  Square(){}

  explicit Square(const float side)
    : side{side}
  {
  }

  string str() const override
  {
    ostringstream oss;
    oss << "A square of with side " << side;
    return oss.str();
  }
};

// we are not changing the base class of existing
// objects

// cannot make, e.g., ColoredSquare, ColoredCircle, etc.

/**
 * 이미 생성된 Shape 객체의 참조를 가지고 새로운 기능을 추가한다.
 * 이러한 추가기능은 그 자체만으로는 사용할 수 없고, 적용할 도형 인스턴스가 있어야만 한다.
 * 중복된 합성도 가능하다 ColoredShape{ ColoredShape{ ...} } 빨간색이면서 노란색..?
 * ColoredShape으로 생성한 circle의 resize()가 가려지는 문제점이 생길 수 있다.
 */

struct ColoredShape : Shape
{
  Shape& shape;
  string color;

  ColoredShape(Shape& shape, const string& color)
    : shape{shape},
      color{color}
  {
  }

  string str() const override
  {
    ostringstream oss;
    oss << shape.str() << " has the color " << color;
    return oss.str();
  }
};

struct TransparentShape : Shape
{
  Shape& shape;
  uint8_t transparency;


  TransparentShape(Shape& shape, const uint8_t transparency)
    : shape{shape},
      transparency{transparency}
  {
  }

  string str() const override
  {
    ostringstream oss;
    oss << shape.str() << " has "
      << static_cast<float>(transparency) / 255.f*100.f
      << "% transparency";
    return oss.str();
  }
};

/**
 * static decorator goes here
 * mixin inheritance: 템플릿 인자로 받은 클래스를 부모 클래스로 지정하는 방식
 */
// mixin inheritance


// note: class, not typename
template <typename T> struct ColoredShape2 : T
{
    /**
     * Shape이외의 타입이 지정되는 것을 막는다.
     */
  static_assert(is_base_of<Shape, T>::value,
    "Template argument must be a Shape");

  string color;

  // need this (or not!)
  ColoredShape2(){}

  template <typename...Args>
  ColoredShape2(const string& color, Args ...args)
    : T(std::forward<Args>(args)...), color{color}
    // you cannot call base class ctor here
    // b/c you have no idea what it is
  {
  }

  string str() const override
  {
    ostringstream oss;
    oss << T::str() << " has the color " << color;
    return oss.str();
  }
};

template <typename T> struct TransparentShape2 : T
{
  uint8_t transparency;

    /**
     * 첫번째 인자는 템플릿 클래스에 적용, 두번째 인자들은 부모 클래스에 전달될 제네릭 파라미터 팩
     * 임의의 개수의 인자를 받을 수 있다. 앞쪽 인자는 투명도 값을 초기화하는 데 이용되고, 나머지 인자들은 그 인자가 어떻게 구성되었냐와 관계없이 단순히 상위 클래스에 전달된다.
     */
  template<typename...Args>
  TransparentShape2(const uint8_t transparency, Args ...args)
    : T(std::forward<Args>(args)...), transparency{ transparency }
  {
  }

  string str() const override
  {
    ostringstream oss;
    oss << T::str() << " has "
      << static_cast<float>(transparency) / 255.f * 100.f
      << "% transparency";
    return oss.str();
  }
};

void wrapper()
{
  Circle circle{ 5 };
  cout << circle.str() << endl;

  ColoredShape red_circle{ circle, "red" };
  cout << red_circle.str() << endl;

  //red_circle.resize(); // oops

  TransparentShape red_half_visible_circle{ red_circle, 128 };
  cout << red_half_visible_circle.str() << endl;
}

void mixin_inheritance()
{
  // won't work without a default constructor
  ColoredShape2<Circle> green_circle{ "green" };
  green_circle.radius = 123;
  cout << green_circle.str() << endl;

  TransparentShape2<ColoredShape2<Square>> blue_invisible_square{ 0 };
  blue_invisible_square.color = "blue";
  blue_invisible_square.side = 321;
  cout << blue_invisible_square.str() << endl;
}

/**
 * Functional Decorator goes here
 * 로깅 기능을 분리하여 다른 부분과 얽히지 않고 재사용과 개선을 마음편히 하자.
 */
struct Logger
{
  function<void()> func;
  string name;

  Logger(const function<void()>& func, const string& name)
    : func{func},
      name{name}
  {
  }

  void operator()() const
  {
    cout << "Entering " << name << endl;
    func();
    cout << "Exiting " << name << endl;
  }
};

/**
 * 코드 블록을 std::function으로서 전달하지 않고 템플릿 인자로 전달할 경우
 * 임의의 코드 블럭을 데커레이션할 수 있고, 데커레이션된 코드 블록을 필요할 때 호출할 수 있다.
 */

template <typename Func>
struct Logger2
{
  Func func;
  string name;

  Logger2(const Func& func, const string& name)
    : func{func},
      name{name}
  {
  }

  void operator()() const
  {
    cout << "Entering " << name << endl;
    func();
    cout << "Exiting " << name << endl;
  }
};

template <typename Func> auto make_logger2(Func func, 
  const string& name)
{
  return Logger2<Func>{ func, name }; 
}

/**
 * 로그를 남기고 싶은 함수의 리턴 값을 넘겨야 할 경우
 */
// need partial specialization for this to work
template <typename> struct Logger3;

// return type and argument list
template <typename R, typename... Args> 
struct Logger3<R(Args...)>
{
  Logger3(function<R(Args...)> func, const string& name)
    : func{func},
      name{name}
  {
  }

  R operator() (Args ...args)
  {
    cout << "Entering " << name << endl;
    R result = func(args...);
    cout << "Exiting " << name << endl;
    return result;
  }

  function<R(Args ...)> func;
  string name;
};

//std::function대신에 일반 함수 포인터를 첫번째 인자로 받고 있다.
template <typename R, typename... Args>
auto make_logger3(R (*func)(Args...), const string& name)
{
  return Logger3<R(Args...)>(
    std::function<R(Args...)>(func), 
    name);
}

double add(double a, double b)
{
  cout << a << "+" << b << "=" << (a + b) << endl;
  return a + b;
}

void function_decorator()
{
  //Logger([]() {cout << "Hello" << endl; }, "HelloFunction")();
  
  // cannot do this
  //make_logger2([]() {cout << "Hello" << endl; }, "HelloFunction")();
  auto call = make_logger2([]() {cout << "Hello!" << endl; }, "HelloFunction");
  call();

  auto logged_add = make_logger3(add, "Add");
  auto result = logged_add(2, 3);
}

void constructor_forwarding()
{
  struct NotAShape
  {
    virtual string str() const { return string{}; }
  };

  // we don't want this to be legal, thus a static_assert above
  //ColoredShape2<NotAShape> legal;

  // no code completion for this case
  // can comment out argument, too! (default constructor)
  TransparentShape2<Square> hidden_square{ 1, 2 };
  cout << hidden_square.str() << endl;

  ColoredShape2<TransparentShape2<Square>> sq = { "red", 51, 5 };
  cout << sq.str() << endl;
}

int main()
{
  function_decorator();
  //wrapper();
  //mixin_inheritance();
  //constructor_forwarding();

  getchar();
  return 0;
}
