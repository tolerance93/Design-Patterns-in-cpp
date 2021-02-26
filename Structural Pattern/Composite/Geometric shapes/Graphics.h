#pragma once
#include <iostream>
#include <vector>
#include <memory>

struct GraphicObject
{
  virtual void draw() = 0;
};

struct Circle : GraphicObject
{
  void draw() override
  {
    std::cout << "Circle" << std::endl;
  }
};

/**
 * 여러 개의 그래픽 객체를 가지는 그룹. 같은 인터페이스를 사용한다.
 */
struct Group : GraphicObject
{
  std::string name;


  explicit Group(const std::string& name)
    : name{name}
  {
  }

  void draw() override
  {
    std::cout << "Group " << name.c_str() << " contains:" << std::endl;
    for (auto&& o : objects)
      o->draw();
  }

  std::vector<GraphicObject*> objects;
};

inline void graphics()
{
  Group root("root");
  Circle c1, c2;
  root.objects.push_back(&c1);

  Group subgroup("sub");
  subgroup.objects.push_back(&c2);

  root.objects.push_back(&subgroup);

  root.draw();
}
