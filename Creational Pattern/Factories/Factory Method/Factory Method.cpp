//cmath의 상수를 이용하기 위해 필요
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>

/**
 * 팩토리와 빌더의 차이점?
 * 빌더는 object를 piecewise로 생성. Factory는 wholesale!
 */

/**
 * https://unikys.tistory.com/376
 * 기존 enum의 타입 비교의 문제점과  scope문제점 해결
 */
enum class PointType
{
  cartesian,
  polar
};

/**
 * Problem
 * (1) 생성자의 이름에는 추가적인 정보를 표시할 수 없다.
 * (2) 생성자 오버로딩으로는 같은 타입을 구분할 수 없다.
 */
class Point
{
  /*Point(float a, float b, PointType type = PointType::cartesian)
  {
  if (type == PointType::cartesian)
  {
  x = a; b = y;
  }
  else
  {
  x = a*cos(b);
  y = a*sin(b);
  }
  }*/


    /**
     * private생성자! 팩토리를 통해서만 생성 가능하도록 한다.
     */
  Point(const float x, const float y)
    : x{x},
      y{y}
  {
  }

public:
  float x, y;


  friend std::ostream& operator<<(std::ostream& os, const Point& obj)
  {
    return os
      << "x: " << obj.x
      << " y: " << obj.y;
  }

    /**
     * 팩토리 메소드
     */
  static Point NewCartesian(float x, float y)
  {
    return{ x,y };
  }
  static Point NewPolar(float r, float theta)
  {
    return{ r*cos(theta), r*sin(theta) };
  }
};

int main()
{
  // will not work
  //Point p{ 1,2 };

  auto p = Point::NewPolar(5, M_PI_4);
  std::cout << p << std::endl;

  getchar();
  return 0;
}
