#include <iostream>
#include <string>
#include <vector>
#include "Person.h"
using namespace std;

// two classes of objects

// Renderers - determine how an object is drawn
// Shapes - determine what to draw

/**
 * interface
 */
struct Renderer
{
  virtual void render_circle(float x, float y, float radius) = 0;
};

struct VectorRenderer : Renderer
{
  void render_circle(float x, float y, float radius) override
  {
    cout << "Rasterizing circle of radius " << radius << endl;
  }
};

struct RasterRenderer : Renderer
{
  void render_circle(float x, float y, float radius) override
  {
    cout << "Drawing a vector circle of radius " << radius << endl;
  }
};

/**
 * renderer를 참조변수로 들고있음. 브릿지!
 * 브릿지 패턴에 연결되는 것들은 서로의 존재를 알아야만 한다.  Circle은 Renderer의 참조를 가져야 하고, Renderer는 원을 어떻게 그리는지 알아야 한다.
 */
struct Shape
{
protected:
  Renderer& renderer;
  Shape(Renderer& renderer) : renderer{ renderer } {}
public:
  virtual void draw() = 0; // implementation specific
  virtual void resize(float factor) = 0; // abstraction specific
};

struct Circle : Shape
{
  float x, y, radius;

    /**
     * 브릿지를 이용해 Circle을 렌더링 절차에 연결하는 부분
     */
  void draw() override
  {
    renderer.render_circle(x, y, radius);
  }

  void resize(float factor) override
  {
    radius *= factor;
  }

  Circle(Renderer& renderer, float x, float y, float radius)
    : Shape{renderer},
      x{x},
      y{y},
      radius{radius}
  {
  }
};

void bridge()
{
  RasterRenderer rr;//렌더러는 자신이 다루는 것이 circle인지 전혀 모르고 자신이 참조 변수로 변수로 접근되고 있다는 것도 알지 못한다.
  Circle raster_circle{ rr, 5,5,5 };
  raster_circle.draw();
  raster_circle.resize(2);
  raster_circle.draw();
}

int main()
{
  // pimpl
  // binary interfaces are fragile; this removes most of the internals to a separate class
  // prevents recompilation of sources reliant on the header

  Person p;
  p.greet();

  getchar();
  return 0;
}
