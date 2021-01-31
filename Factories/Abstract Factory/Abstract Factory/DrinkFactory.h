#pragma once
#include <string>
#include "HotDrink.h"
#include "TeaFactory.h"
#include "CoffeeFactory.h"
#include <map>
#include <functional>

struct HotDrink;

/**
 * 상위 수준에서 다른 종류의 음료를 만들어야 한다면!
 * 사용 가능한 다양한 팩토리들에 대한 참조를 내부에 가지도록 한다.
 */
class DrinkFactory
{
  map<string, unique_ptr<HotDrinkFactory>> hot_factories;
public:
  DrinkFactory()
  {
      /**
       * 객체 자체가 아니라 스마트 포인터로 저장.
       * 포인터 대신 객체 값을 직접 저장하면 저장소의 타입에 따라 객체 슬라이싱 문제가 발생할 수 있다?
       * 객체 슬라이싱? https://stackoverflow.com/questions/274626/what-is-object-slicing
       */
    hot_factories["coffee"] = make_unique<CoffeeFactory>();
    hot_factories["tea"] = make_unique<TeaFactory>();
  }

  unique_ptr<HotDrink> make_drink(const string& name)
  {
    auto drink = hot_factories[name]->make();
    drink->prepare(200); // oops!
    return drink;
  }
};

class DrinkWithVolumeFactory
{
  map<string, function<unique_ptr<HotDrink>()>> factories;
public:

  DrinkWithVolumeFactory()
  {
    factories["tea"] = [] {
      auto tea = make_unique<Tea>();
      tea->prepare(200);
      return tea;
    };
  }

  unique_ptr<HotDrink> make_drink(const string& name);
};

inline unique_ptr<HotDrink> DrinkWithVolumeFactory::make_drink(const string& name)
{
  return factories[name]();
}
