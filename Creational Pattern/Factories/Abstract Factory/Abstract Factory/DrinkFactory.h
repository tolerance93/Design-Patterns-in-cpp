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

/**
 * 함수형 팩토리
 */
class DrinkWithVolumeFactory
{
    //HotDrink: return, with no argument
  map<string, function<unique_ptr<HotDrink>()>> factories;
public:

  DrinkWithVolumeFactory()
  {
      //lamda factory method
      //https://stackoverflow.com/questions/7627098/what-is-a-lambda-expression-in-c11
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
    //함수를 invoke하기위해 ()
  return factories[name]();
}

/**
 * 팩토리가 생성자보다 나은 점들
 * (1) 팩토리는 객체의 생성을 거부할 수 있다.
 * (2) 팩토리는 가독성 높은 명명이 가능하다.
 * (3) 단일 팩토리가 서로 다른 여러 타입의 객체를 생성할 수 있다.
 * (4) 팩토리에 다형성을 부여할 수 있다.
 */

/**
 * 팩토리 VS 빌더
 * 팩토리는 객체를 한번에 생성하지만, 빌더는 각 구성요소마다 필요한 정보를 제공하며 여러 단계를 거쳐 객체를 생성한다.
 */
