/**
 * 여러 종류의 연관된 객체들을 생성해야 할 경우
 * 복잡한 시스템에서 사용
 */

#include <iostream>
#include <memory>
#include <map>
#include "HotDrink.h"
#include "DrinkFactory.h"
using namespace std;

unique_ptr<HotDrink> make_drink(string type)
{
  unique_ptr<HotDrink> drink;
  if (type == "tea")
  {
    drink = make_unique<Tea>();
    drink->prepare(200);
  }
  else 
  {
    drink = make_unique<Coffee>();
    drink->prepare(50);
  }
  return drink;
}

int main()
{
  auto d = make_drink("tea");

  DrinkFactory df;
  df.make_drink("coffee");

  getchar();
  return 0;
}
