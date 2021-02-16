#pragma once
#include "HotDrink.h"

/**
 * 여기가 추상 팩토리.
 * 어떤 특정 인터페이스를 구현하고 있지만 구현 클래스가 아니라 추상 클래스이다.
 */
struct HotDrinkFactory
{
  virtual unique_ptr<HotDrink> make() const = 0;
};
