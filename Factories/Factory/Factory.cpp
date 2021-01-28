#include <cmath>

/**
 * GoF에는 팩토리 메소드와 추상 팩토리가 있을뿐 팩토리는 없다.
 * idea: Point안에 팩토리 메소드를 두지 않고, 생성만을 전담하는 클래스를 만들자!
 */
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
     * 생성자를 Public으로 변경해서 팩토리 클래스를 프렌드로 만들지 않을 수 있다.
     * OCP는 위반하지 않지만, 클라이언트가 팩토리와 생성자중 무엇을 사용해야하는지 혼란을 줄 수 있다.
     */
  Point(float x, float y) : x(x), y(y){}
public:
  float x, y;

    /**
     * Open Closed Principe을 위반할 수 있다!
     * 이미 만든 Point Class에 팩토리를 추가할 경우..
     * 위반하지 않으려면 포인트 클래스와 포인트팩토리 클래스가 동시에 만들어져야 한다.
     */
  friend class PointFactory;
};

class PointFactory
{
  static Point NewCartesian(float x, float y)
  {
    return Point{ x,y };
  }

  static Point NewPolar(float r, float theta)
  {
    return Point{ r*cos(theta), r*sin(theta) };
  }
};
