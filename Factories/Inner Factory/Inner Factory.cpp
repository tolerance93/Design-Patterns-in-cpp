#include <cmath>
/**
 * C#, Java등 프렌드 키워드가 없는 언어들은 내부 팩토리를 흔하게 사용한다.
 */
// do not need this for factory
enum class PointType
{
  cartesian,
  polar
};

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

  // use a factory method
    /**
     * 사용자 입장에서는 생성자가 private이라면 Point::를 통해 사용가능한 수단이 무엇이 있는지 확인할 것이고,
     * 팩토리는 그러한 사용자의 기대에 부흥한다!
     */
  Point(float x, float y) : x(x), y(y) {}
/**
 * Point와 PointFactory는 서로의 private member에 자유로운 접근권한을 가진다.
 * 팩토리가 생성해야할 클래스가 단 한 종류일 때 유용하다.
 */
  class PointFactory
  {
    PointFactory() {}
  public:
    static Point NewCartesian(float x, float y)
    {
      return { x,y };
    }
    static Point NewPolar(float r, float theta)
    {
      return{ r*cos(theta), r*sin(theta) };
    }
  };
public:
  float x, y;
  static PointFactory Factory;
};

int main()
{
  // will not work
  // Point p{ 1,2 };

  // nope!
  // Point::PointFactory pf;

  // if factory is public, then
  //auto p = Point::PointFactory::NewCartesian(3, 4);

  // at any rate, use this
  auto pp = Point::Factory.NewCartesian(2, 3);

  return 0;
}
